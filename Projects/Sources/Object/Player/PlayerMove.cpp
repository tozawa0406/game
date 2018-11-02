#include "PlayerMove.h"
#include <FrameWork/Systems/Input/Controller.h>
#include "../../Scene/EachScene/GameScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Object/ObjectManager.h>
#include <random>

#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>

//! @def	移動速度
static constexpr float MOVE_SPEED = 0.06f;
//! @def	大きさ
static constexpr float SCALE = 0.1f;

//! @def	体のボーンの名前
static const     string BONE_BODY_NAME = "Character1_Hips";
//! @def	右手のボーンの名前
static const     string BONE_RIGHT_HAND_NAME = "Character1_RightHand";
//! @def	当たり判定のオフセット位置
static const     VECTOR3 COLLISION_OFFSET = VECTOR3(0, 7.5f, 0);
//! @def	当たり判定のサイズ
static const     VECTOR3 COLLISION_SIZE = VECTOR3(3, 15, 3);

//! @def	アニメーション変更速度
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;

//! @def	待機モーションの再生待ち時間
static constexpr int    ANIMATION_WAIT_FRAME = 180;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
PlayerMove::PlayerMove(void) : Object(Object::Tag::PLAYER), GUI(Systems::Instance(), this, "player")
	, animCnt_(ANIMATION_DEFAULT)
	, body_(nullptr)
	, hand_(nullptr)
	, velocity_(VECTOR3(0))
	, avoidanceDir_(VECTOR3(0))
	, inputDir_(VECTOR2(0))
	, inputDash_(0)
	, front_(VECTOR3(0))
	, cameraManager_(nullptr)
	, camera_(nullptr)
	, flag_(0)
	, wapon_(nullptr)
	, collider_(nullptr)
	, waitTime_(0)
	, animation_(Animation::Wait)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
PlayerMove::~PlayerMove(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし
 * @detail	メッシュや当たり判定などの初期化		*/
void PlayerMove::Init(void)
{
	const auto& systems = Systems::Instance();
	if (!systems) { return; }

	// メッシュ
	mesh_.Init(systems, static_cast<int>(Model::Game::UNITY_CHAN), &transform_);
	mesh_.shader = Shader::ENUM::DEFAULT;

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
			const auto& model = wrapper->GetModel(mesh_.GetModelNum());

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
	collider_->SetOffset(COLLISION_OFFSET);
	collider_->SetSize(COLLISION_SIZE);
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void PlayerMove::Uninit(void)
{
	// 生成したTPSカメラの後始末
	if (cameraManager_ && camera_)
	{
		cameraManager_->DestroyObjCamera(camera_);
	}
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	なし					*/
void PlayerMove::Update(void)
{
	BitSub(flag_, IS_END_ANIMATION);
	if (mesh_.Animation(animCnt_))
	{
		BitAdd(flag_, IS_END_ANIMATION);
	}
	StandBy();

	Move();

	OnGround();

	collider_->Update();

//	mesh_.Skinning();
}

/* @fn		StandBy
 * @brief	待機モーション
 * @sa		Update()
 * @param	なし
 * @return	なし					*/
void PlayerMove::StandBy(void)
{
	// 元に戻る
	if (((animation_ == Animation::WaitTime1) || (animation_ == Animation::WaitTime2)) || 
		((animation_ == Animation::Setup)     || (animation_ == Animation::Roll)) && BitCheck(flag_, IS_END_ANIMATION))
	{
		// 納刀状態と抜刀状態でアニメーションの切り替え
		animation_ = (BitCheck(flag_, IS_DRAWN)) ? Animation::SetupWait : Animation::Wait;
		// 再生速度の設定
		animCnt_ = ANIMATION_DEFAULT;

		// アニメーションの変更
		mesh_.ChangeAnimation(static_cast<int>(animation_), ANIMATION_CHANGE_FRAME30, true);
	}

	// 抜刀中は待機モーションなし
	if (BitCheck(flag_, IS_DRAWN)) { return; }

	// 納刀時の待機中
	if (animation_ == Animation::Wait)
	{
		waitTime_++;
		// 一定時間を超えたら待機モーションの再生
		if (waitTime_ > ANIMATION_WAIT_FRAME)
		{
			// 2種類のうちどちらか
			std::random_device dev;
			animation_ = (Animation)(static_cast<int>(Animation::WaitTime1) + (dev() % 2));
			mesh_.ChangeAnimation(static_cast<int>(animation_), ANIMATION_CHANGE_FRAME30);
		}
	}
	else 
	{
		// 待機時間の初期化
		waitTime_ = 0; 
	}
}

/* @fn		Move
 * @brief	移動処理
 * @sa		Update()
 * @param	なし
 * @return	なし
 * @detail	入力検知、アニメーション切り替え、
			前ベクトルの生成、向きの変更、衝突判定		*/
void PlayerMove::Move(void)
{
	// Input
	inputDir_.x = (float)GetCtrl(0)->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
	inputDir_.y = (float)GetCtrl(0)->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
	// 正規化
	inputDir_ = VecNorm(inputDir_);

	inputDash_  = (GetCtrl(0)->Press(Input::GAMEPAD_R1, DIK_LSHIFT)) ? 2.0f : 1.0f;;

	// 納刀抜刀中は移動無し
	if (!BitCheck(flag_, IS_AVOIDANCE | IS_SETUP | IS_ATTACK))
	{
		// 走り判定
		if (BitCheck(flag_, IS_DRAWN)) { inputDash_ = 0.75f; }
		inputDir_ *= inputDash_;

		// アニメーション切り替え
		if (fabs(inputDir_.x) + fabs(inputDir_.y) > 0)
		{
			animCnt_   = 0.75f;
			animation_ = (inputDash_ <= 1) ? (BitCheck(flag_, IS_DRAWN)) ? Animation::SetupWalk : Animation::Walk : Animation::Run;
			int cnt = (animation_ == Animation::Run) ? 15 : ANIMATION_CHANGE_FRAME30;
			mesh_.ChangeAnimation((int)animation_, cnt);
		}
		else if ((animation_ == Animation::Walk || animation_ == Animation::Run) || animation_ == Animation::SetupWalk)
		{
			animation_ = (BitCheck(flag_, IS_DRAWN)) ? Animation::SetupWait : Animation::Wait;
			int cnt = (animation_ == Animation::Run) ? 15 : ANIMATION_CHANGE_FRAME30;
			mesh_.ChangeAnimation((int)animation_, cnt);
		}
		// 移動速度
		if (camera_)
		{
			velocity_ += camera_->GetFrontXPlane() * inputDir_.y * MOVE_SPEED;
			velocity_ -= camera_->GetRightXPlane() * inputDir_.x * MOVE_SPEED;
		}
	}

	// キャラクターの前ベクトルの生成
	CreateFrontVector();

	if (BitCheck(flag_, IS_AVOIDANCE))
	{
		velocity_ += avoidanceDir_ * MOVE_SPEED * 2.25f;
	}

	// 移動向きによりキャラクターの向きを変える
	if ((Abs(velocity_.x) + Abs(velocity_.z) > 0.02f))
	{
		VECTOR3 velocityNorm	= VecNorm(velocity_);
		VECTOR3 frontVelocityCross		= VecCross(front_, velocityNorm);
		float	dot = VecDot(front_, velocityNorm);

		// 前か後ろに進みたいとき
		int sign = 1;
		if (frontVelocityCross.y < 0) { sign = -1; }

		int upY = (int)(((frontVelocityCross.y * 10) + (5 * sign)) * 0.1f);
		// 内積が0以下の時(後ろに進むとき)
		if (upY == 0 && dot == 1)
		{
			// 強制的に回す
			frontVelocityCross.y = 1.0f * sign;
		}

		transform_.rotation.y += frontVelocityCross.y * 0.3f;
	}
	else { velocity_ = VECTOR3(0); }

	transform_.position += velocity_;		// 移動

	const auto& hits = collider_->Hit();
	for (auto& hit : hits)
	{
		if (hit->GetTag() == Object::Tag::STATIC)
		{
			this->transform_.position -= velocity_;
		}
	}

	velocity_ *= 0.8f;						// 慣性
}

/* @fn		CreateFrontVector
 * @brief	前ベクトルの生成
 * @sa		Move()
 * @param	なし
 * @return	なし					*/
void PlayerMove::CreateFrontVector(void)
{
	MATRIX frontObj;
	frontObj.Identity().Translation(VECTOR3(0, 0, 1));
	MATRIX mtx;
	mtx.Identity().Rotation(VECTOR3(0, transform_.rotation.y, 0));
	mtx.Translation(transform_.position);
	frontObj *= mtx;

	VECTOR3 tempTarget = VECTOR3(frontObj._41, transform_.position.y, frontObj._43);
	front_ = transform_.position - tempTarget;
	front_ = VecNorm(front_);
}

/* @fn		OnGround
 * @brief	接地判定
 * @sa		Update()
 * @param	なし
 * @return	なし					*/
void PlayerMove::OnGround(void)
{
	transform_.position.y = 0;
	if (manager_)
	{
		if (const auto& scene = static_cast<GameScene*>(manager_->GetScene()))
		{
			if (const auto& meshfield = scene->GetMeshField())
			{
				float y = meshfield->Hit(transform_.position);
				if (y > 0) { transform_.position.y += y; }
			}
		}
	}
}

/* @fn		GuiUpdate
 * @brief	デバッグ用描画更新
 * @param	なし
 * @return	なし					*/
void PlayerMove::GuiUpdate(void)
{
	ImGui::Text("state : ");
	ImGui::SameLine();

	if (BitCheck(flag_, IS_DRAWN))
	{
		ImGui::Text("drawn");
		ImGui::SameLine();
	}
	if (BitCheck(flag_, IS_SETUP))
	{
		ImGui::Text("setup");
		ImGui::SameLine();
	}
	if (BitCheck(flag_, IS_AVOIDANCE))
	{
		ImGui::Text("avoidance");
		ImGui::SameLine();
	}
	if (BitCheck(flag_, IS_ATTACK))
	{
		ImGui::Text("attack");
		ImGui::SameLine();
	}

	ImGui::Text(" ");



	ImGui::Text("front : %.2f, %.2f, %.2f", front_.x, front_.y, front_.z);
	ImGui::Text("veloc : %.2f, %.2f, %.2f", velocity_.x, velocity_.y, velocity_.z);

	ImGui::Text("animation    : %d", mesh_.GetAnimation());
	ImGui::Text("animationOld : %d", mesh_.GetAnimationOld());

	ImGui::Text("pattern      : %.2f", mesh_.GetPattern());
}
