#include "Player.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Object/ObjectManager.h>
#include <random>

#include <FrameWork/Graphics/DirectX11/Dx11Wrapper.h>
#include "PlayerState/PlayerState.h"
#include "PlayerState/PaidState/PaidWaitState.h"

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

Player::Player(const VECTOR3& position) : GameObject(ObjectTag::PLAYER), GUI(Systems::Instance(), this, "player")
	, state_(nullptr)
	, stamina_(150)
	, maxLife_(100)
	, maxStamina_(25)
	, isDraw_(false)
	, body_(nullptr)
	, hand_(nullptr)
	, cameraManager_(nullptr)
	, camera_(nullptr)
	, wapon_(nullptr)
	, collider_(nullptr)
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
	}

	state_ = new PaidWaitState;
	if (state_)
	{
		state_->Init(this, GetCtrl(0));
	}
}

void Player::Uninit(void)
{
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

}

void Player::Hit(int damage)
{
	if (collider_->IsEnable())
	{
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
}

void Player::Earplug(uint8 attack)
{
	if (hitAttack_ == attack) { return; }

	hitAttack_ = attack;
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
