#include "Player.h"
#include <FrameWork/Systems/Input/Controller.h>
#include "../../../Scene/GameScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Object/ObjectManager.h>
#include <random>

#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>
#include "PlayerState/PlayerState.h"
#include "PlayerState/PaidState/PaidWaitState.h"

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

/* @brief	コンストラクタ			*/
Player::Player(void) : GameObject(Object::Tag::PLAYER), GUI(Systems::Instance(), this, "player")
	, state_(nullptr)
	, stamina_(150)
	, isDraw_(false)
	, body_(nullptr)
	, hand_(nullptr)
	, cameraManager_(nullptr)
	, camera_(nullptr)
	, wapon_(nullptr)
	, collider_(nullptr)
{
	meshAnim_.animation = static_cast<int>(Animation::Wait);
	meshAnim_.animSpeed = ANIMATION_DEFAULT;

	life_ = 150;
}

/* @brief	デストラクタ			*/
Player::~Player(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	なし					*/
void Player::Init(void)
{
	const auto& systems = Systems::Instance();
	if (!systems) { return; }

	// メッシュ
	meshAnim_.mesh.Init(systems, static_cast<int>(Model::Game::UNITY_CHAN), &transform_);
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


	if (const auto& renderer = systems->GetRenderer())
	{
		if (DirectX11Wrapper* wrapper = static_cast<DirectX11Wrapper*>(renderer->GetWrapper()))
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

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
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

/* @brief	更新処理
 * @param	なし
 * @return	なし					*/
void Player::Update(void)
{
	if (IsDed()) { return; }

	isEndAnim_ = meshAnim_.mesh.Animation(meshAnim_.animSpeed);

#ifdef _SELF_DEBUG	
	COLOR c = COLOR(1, 1, 1, 1);
	const auto& hits = collider_->HitCollider();
	for (auto& hit : hits)
	{
		if (hit->GetRendererColor() == COLOR(1, 0, 0, 1))
		{
			if (hit->GetParent()->GetTag() == Object::Tag::ENEMY)
			{
				c = COLOR(1, 0, 0, 1);
			}
		}
	}
//	meshAnim_.mesh.material.diffuse = c;
#endif

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

	stamina_ = min(stamina_ + ADD_STAMINA, 150);

	transform_.position.y = 0;
	Move();

	transform_.position += collider_->GetBack();
}

/* @brief	ダメージ処理
 * @param	(damage)	ダメージ
 * @return	なし			*/
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

/* @brief	死亡判定
 * @sa		Update()
 * @param	なし
 * @return	死んでいたらtrue			*/
bool Player::IsDed(void)
{
	if (static_cast<Animation>(meshAnim_.animation) == Animation::Die)
	{
		int maxAnim = meshAnim_.mesh.GetMaxAnimation();
		int pattern = static_cast<int>(meshAnim_.mesh.GetPattern());
		if (pattern >= maxAnim - 1)
		{
			return true;
		}
	}
	return false;
}

/* @brief	デバッグ用描画更新
 * @param	なし
 * @return	なし					*/
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
