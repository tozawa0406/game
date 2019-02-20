#include "Player.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Object/ObjectManager.h>
#include <random>

#include <FrameWork/Graphics/DirectX11/Dx11Wrapper.h>
#include "PlayerState/PlayerState.h"

#include "../../UI/PlayerLife.h"
#include "../../UI/ItemList.h"

#include "PlayerState/PaidState/PaidWaitState.h"
#include "PlayerState/DrawnState/DrawnWaitState.h"

#include "PlayerState/EarplugState.h"
#include "PlayerState/KnockBackState.h"
#include "PlayerState/DieState.h"

//! @def	大きさ
static constexpr float SCALE = 0.1f;

//! @def	体のボーンの名前
static const     string BONE_BODY_NAME = "Character1_Hips";
//! @def	右手のボーンの名前
static const     string BONE_RIGHT_HAND_NAME = "Character1_RightHand";
//! @def	当たり判定のオフセット位置
static const     VECTOR3 COLLISION_OFFSET_POS = VECTOR3(0, 7.5f, 0);
//! @def	当たり判定のサイズ
static const     VECTOR3 COLLISION_SIZE = VECTOR3(3, 15, 3);

//! @def	スタミナ減少時間(分単位)
static constexpr int     STAMINA_DOWN_TIME = 5;

Player::Player(const VECTOR3& position) : GameObject(ObjectTag::PLAYER), GUI(Systems::Instance(), this, "player")
	, itemList_(nullptr)
	, state_(nullptr)
	, stamina_(150)
	, staminaCnt_(0)
	, maxLife_(100)
	, maxStamina_(100)
	, isDraw_(false)
	, body_(nullptr)
	, hand_(nullptr)
	, cameraManager_(nullptr)
	, camera_(nullptr)
	, wapon_(nullptr)
	, collider_(nullptr)
	, colliderDefense_(nullptr)
{
	transform_.position = position;

	meshAnim_.animation = static_cast<int>(Animation::Wait);
	meshAnim_.animSpeed = ANIMATION_DEFAULT;

	life_ = min(100, maxLife_);
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	const auto& systems = Systems::Instance();
	if (!systems) { return; }

	if (const auto& lifeGauge = manager_->Create<PlayerLife>())
	{
		lifeGauge->SetPlayer(this);
	}

	itemList_ = manager_->Create<ItemList>();

	// メッシュ
	meshAnim_.mesh.Init(systems, static_cast<int>(Resources::Model::Camp::UNITY_CHAN), &transform_);
	meshAnim_.mesh.shader = Shader::ENUM::DEFAULT;

	// スケールの設定
	transform_.scale = VECTOR3(SCALE);

	// カメラマネージャーの取得
	if (manager_)
	{
		if (const auto& scene = manager_->GetScene())
		{
			if (const auto& sceneManager = scene->GetManager())
			{
				if (const auto& cameraManager = sceneManager->GetCameraManager())
				{
					cameraManager_ = cameraManager;
				}
			}
		}
	}
	assert(cameraManager_);

	// TPSカメラの生成
	if (cameraManager_)
	{
		camera_ = cameraManager_->CreateAtObjCamera(this, 0);
	}


	if (const auto& renderer = systems->GetGraphics())
	{
		if (Dx11Wrapper* wrapper = static_cast<Dx11Wrapper*>(renderer->GetWrapper()))
		{
			// 武器
			const auto& model = wrapper->GetModel(meshAnim_.mesh.GetModelNum());

			for (auto& bone : model.bone)
			{
				if (bone.name == BONE_RIGHT_HAND_NAME)
				{
					hand_ = &bone.nowBone;
				}
				else if (bone.name == BONE_BODY_NAME)
				{
					body_ = &bone.nowBone;
				}
			}
		}
	}

	// 当たり判定
	collider_ = new Collider3D::OBB(this);
	if (collider_)
	{
		collider_->SetOffsetPosition(COLLISION_OFFSET_POS);
		collider_->SetSize(COLLISION_SIZE);
		colliderDefense_ = new Collider3D::OBB(this);
		CreateDefenseCollider(*collider_, colliderDefense_);
	}

	if (manager_)
	{
		if (const auto& scene = manager_->GetScene())
		{
			if (const auto& sceneManager = scene->GetManager())
			{
				if (const auto& load = sceneManager->GetDontDestroyOnLoad())
				{
					int i = load->Load<int>("MaxLife", 0);
					if (i > 0) { maxLife_ = i; }
					i = load->Load<int>("CurrentLife", 0);
					if (i > 0) { life_ = i; }

					float f = load->Load<float>("MaxStamina", 0);
					if (f > 0) { maxStamina_ = f; }
					f = load->Load<float>("CurrentStamina", 0);
					if (f > 0) { stamina_ = f; }

					i = load->Load<int>("StaminaCnt", 0);
					if (i > 0) { staminaCnt_ = i; }

					i = load->Load<int>("PlayerState", 0);
					if (i == 0) { state_ = new PaidWaitState;  }
					else		
					{
						isDraw_ = true;
						state_ = new DrawnWaitState; 
					}
				}
			}
		}
	}
	if (state_)
	{
		state_->Init(this, GetCtrl(0));
	}

}

void Player::Uninit(void)
{
	if (manager_)
	{
		if (const auto& scene = manager_->GetScene())
		{
			if (const auto& sceneManager = scene->GetManager())
			{
				if (const auto& load = sceneManager->GetDontDestroyOnLoad())
				{
					load->Save("MaxLife", maxLife_);
					load->Save("CurrentLife", life_);

					load->Save("MaxStamina", maxStamina_);
					load->Save("CurrentStamina", stamina_);

					load->Save("StaminaCnt", staminaCnt_);

					int saveData = (isDraw_) ? 1 : 0;
					load->Save("PlayerState", saveData);
				}
			}
		}
	}

	DeletePtr(colliderDefense_);
	DeletePtr(collider_);
	// 生成したTPSカメラの後始末
	if (cameraManager_ && camera_)
	{
		cameraManager_->DestroyObjCamera(camera_);
	}
	DeletePtr(state_);
}

void Player::Update(void)
{
	if (IsDedJudge()) { return; }

	isEndAnim_ = meshAnim_.mesh.Animation(meshAnim_.animSpeed);

	if (state_)
	{
		const auto& temp = state_->Update();
		if (temp)
		{
			UninitDeletePtr(state_);
			temp->Init(this, GetCtrl(0));
			state_ = temp;
		}
	}
	stamina_ = min(stamina_ + ADD_STAMINA, maxStamina_);

	transform_.position.y = 0;
	Move();

	transform_.position += collider_->GetBack();

	// スタミナの最大値減少
	staminaCnt_++;
	if (staminaCnt_ > STAMINA_DOWN_TIME * 60 * 60)
	{
		staminaCnt_ = 0;
		maxStamina_ = max(maxStamina_ - 25, 25);
	}
}

void Player::Hit(int damage, uint8 attackID)
{
	if (UpdateHitAttackID(attackID)) { return; }

	if (colliderDefense_->IsEnable())
	{
		if (meshAnim_.animation == static_cast<int>(Animation::Die)) { return; }

		life_ -= damage;
		UninitDeletePtr(state_);
		if (life_ > 0)
		{
			state_ = new KnockBackState;
		}
		else
		{
			state_ = new DieState;
		}
		state_->Init(this, GetCtrl(0));
	}

	if (const auto& systems = Systems::Instance())
	{
		if (const auto& sound = systems->GetSound())
		{
			sound->Play(static_cast<int>(Resources::Sound::Camp::UNITYCHAN_HIT));
		}
	}
}

void Player::Earplug(uint8 attackID)
{
	if (UpdateHitAttackID(attackID)) { return; }

	UninitDeletePtr(state_);
	state_ = new EarplugState;
	if (state_)
	{
		state_->Init(this, GetCtrl(0));
	}
}

bool Player::IsDedJudge(void)
{
	if (ded_) { return true; }

	if (static_cast<Animation>(meshAnim_.animation) == Animation::Die)
	{
		int maxAnim = meshAnim_.mesh.GetMaxAnimation();
		int pattern = static_cast<int>(meshAnim_.mesh.GetPattern());
		if (pattern >= maxAnim - 2)
		{
			ded_ = true;
			return true;
		}
	}
	return false;
}

void Player::GuiUpdate(void)
{
	ImGui::Text("temp : %.2f, %.2f, %.2f", collider_->GetBack().x, collider_->GetBack().y, collider_->GetBack().z);

	ImGui::Text("Life : %d", life_);
	ImGui::Text("Drawn : ");
	ImGui::SameLine();
	if (isDraw_)
	{
		ImGui::Text("true");
	}
	else
	{
		ImGui::Text("false");
	}

	ImGui::Text("state : ");
	if (state_)
	{
		ImGui::SameLine();
		state_->GuiUpdate(); 
	}

	ImGui::Text("front : %.2f, %.2f, %.2f", front_.x, front_.y, front_.z);
	ImGui::Text("veloc : %.2f, %.2f, %.2f", velocity_.x, velocity_.y, velocity_.z);

	ImGui::Text("animation    : %d", meshAnim_.mesh.GetAnimation());
	ImGui::Text("animationOld : %d", meshAnim_.mesh.GetAnimationOld());

	ImGui::Text("pattern      : %.2f", meshAnim_.mesh.GetPattern());
}
