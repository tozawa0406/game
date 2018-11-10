//-----------------------------------------------------------------------------
//
//	Auther : 戸澤翔太
//-----------------------------------------------------------------------------
#include "Dragon.h"
#include <FrameWork/Object/ObjectManager.h>
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>
#include "../../../Scene/GameScene.h"

#include "Attack/DragonScream.h"
#include "Attack/DragonBite.h"
#include "Attack/DragonWingAttack.h"
#include "Attack/DragonTakeOff.h"

//! @def	大きさ
static constexpr float SCALE = 0.9f;
//! @def	移動速度
static constexpr float MOVE_SPEED = 0.03f;


//! @def	走りフラグ
static constexpr uint IS_DASH = 0x0001;
//! @def	飛行フラグ
static constexpr uint IS_FLY = 0x0002;

//! @def	体のボーンの名前
static const     string BONE_BODY = "Spine2";
static const     VECTOR3 COLLISION_OFFSET_POS_BODY = VECTOR3(-4, 1.5f, 0);
static const     VECTOR3 COLLISION_SIZE_BODY = VECTOR3(18, 9, 15);
//! @def	頭のボーンの名前
static const     string BONE_HEAD = "Head";
static const     VECTOR3 COLLISION_OFFSET_POS_HEAD = VECTOR3(5, 3, 0);
static const     VECTOR3 COLLISION_SIZE_HEAD = VECTOR3(6, 6, 6);
//! @def	首1のボーン(体より)
static const     string BONE_NECK1 = "Neck2";
static const     VECTOR3 COLLISION_OFFSET_POS_NECK1 = VECTOR3(1, 2, 0);
static const     VECTOR3 COLLISION_SIZE_NECK1 = VECTOR3(8, 8, 10);
//! @def	首2のボーン(頭より)
static const     string BONE_NECK2 = "Neck4";
static const     VECTOR3 COLLISION_OFFSET_POS_NECK2 = VECTOR3(3, 1, 0);
static const     VECTOR3 COLLISION_SIZE_NECK2 = VECTOR3(10, 5, 5);
//! @def	頭のボーンの名前
static const     string BONE_WIND_L = "WingClaw2_L";
static const     VECTOR3 COLLISION_OFFSET_POS_WING_L = VECTOR3(0, -3, -4);
static const     VECTOR3 COLLISION_OFFSET_ROT_WING_L = VECTOR3(0, -2, 0);
static const     VECTOR3 COLLISION_SIZE_WING_L = VECTOR3(7, 2, 2);

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
Dragon::Dragon(void) : Object(Object::Tag::ENEMY), GUI(Systems::Instance(), this, "dragon")
	, velocity_(VECTOR3(0))
	, front_(VECTOR3(0))
	, right_(VECTOR3(0))
	, animation_(Animation::WAIT)
	, animSpeed_(0)
	, flag_(0)
	, debugMove_(false)
	, currentAttack_(nullptr)
	, cameraManager_(nullptr)
	, camera_(nullptr)
{
	for (auto& c : collision_)
	{
		c = nullptr;
	}

	for (auto& a : attack_)
	{
		a = nullptr;
	}
}

/* @fn		デストラクタ
 * @brief	...						*/
Dragon::~Dragon(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし					*/
void Dragon::Init(void)
{
	transform_.position		= VECTOR3(0, 0, 50);
	transform_.rotation.y   = PI;
	transform_.scale		= VECTOR3(SCALE);

	animSpeed_ = 0.75f;

	mesh_.Init(Systems::Instance(), (int)Model::Game::DRAGON, &transform_);

	CreateCollision();
	
	int arrayNum = static_cast<int>(AttackPattern::SCREAM);
	attack_[arrayNum] = new DragonScream;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	arrayNum = static_cast<int>(AttackPattern::BITE);
	attack_[arrayNum] = new DragonBite;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	arrayNum = static_cast<int>(AttackPattern::WING_ATTACK);
	attack_[arrayNum] = new DragonWingAttack;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	arrayNum = static_cast<int>(AttackPattern::TAKE_OFF);
	attack_[arrayNum] = new DragonTakeOff;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

#ifdef _SELF_DEBUG
	// デバッグ用、敵操作時のカメラ
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
		camera_ = cameraManager_->CreateAtObjCamera(this, 1, VECTOR3(0, 25, -30));
	}
#endif

}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void Dragon::Uninit(void)
{
	for (auto& c : collision_)
	{
		DeletePtr(c);
	}

	for (auto& a : attack_)
	{
		UninitDeletePtr(a);
	}

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
void Dragon::Update(void)
{
#ifdef _SELF_DEBUG
	DebugInput();
#endif

	bool change = mesh_.Animation(animSpeed_);

	if (currentAttack_)
	{
		int temp = static_cast<int>(animation_);
		if (currentAttack_->Update(transform_, velocity_, mesh_, animSpeed_, temp, change))
		{
			currentAttack_ = nullptr;
		}
		animation_ = static_cast<Animation>(temp);
	}

	Move();

	if (!BitCheck(flag_, IS_FLY) && animation_ != Animation::TAKE_OFF)
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

	COLOR color = COLOR(1, 1, 1, 1);
	for (auto& c : collision_)
	{
		if (c)
		{
			c->Update();

			auto hits = c->Hit();
			for (auto& hit : hits)
			{
				const auto& t = hit->GetTag();
				if (t == Object::Tag::WAPON)
				{
					color = COLOR(1, 0, 0, 1);
				}
				if (t == Object::Tag::PLAYER)
				{
					color = COLOR(0, 0, 0, 1);
				}
			}
		}
	}
	mesh_.material.diffuse = color;
}

/* @fn		CreateCollsion
 * @brief	当たり判定生成処理
 * @sa		Init
 * @param	なし
 * @return	なし					*/
void Dragon::CreateCollision(void)
{
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& renderer = systems->GetRenderer())
		{
			if (DirectX11Wrapper* wrapper = static_cast<DirectX11Wrapper*>(renderer->GetWrapper()))
			{
				const auto& model = wrapper->GetModel(mesh_.GetModelNum());

				auto  s = transform_.scale;
				int num = static_cast<int>(Collision::BODY);
				collision_[num] = new Collider3D::OBB(this);
				if (collision_[num])
				{
					for(auto& bone : model.bone)
					{
						if (bone.name == BONE_BODY)
						{
							collision_[num]->SetParentMtx(&model.transMtx, &bone.nowBone);
							break;
						}
					}
					collision_[num]->SetOffsetPosition(COLLISION_OFFSET_POS_BODY * s);
					collision_[num]->SetSize(COLLISION_SIZE_BODY * s);
				}

				num = static_cast<int>(Collision::HEAD);
				collision_[num] = new Collider3D::OBB(this);
				if (collision_[num])
				{
					for (auto& bone : model.bone)
					{
						if (bone.name == BONE_HEAD)
						{
							collision_[num]->SetParentMtx(&model.transMtx, &bone.nowBone);
							break;
						}
					}
					collision_[num]->SetOffsetPosition(COLLISION_OFFSET_POS_HEAD * s);
					collision_[num]->SetSize(COLLISION_SIZE_HEAD * s);
				}

				num = static_cast<int>(Collision::NECK1);
				collision_[num] = new Collider3D::OBB(this);
				if (collision_[num])
				{
					for (auto& bone : model.bone)
					{
						if (bone.name == BONE_NECK1)
						{
							collision_[num]->SetParentMtx(&model.transMtx, &bone.nowBone);
							break;
						}
					}
					collision_[num]->SetOffsetPosition(COLLISION_OFFSET_POS_NECK1 * s);
					collision_[num]->SetSize(COLLISION_SIZE_NECK1 * s);
				}

				num = static_cast<int>(Collision::NECK2);
				collision_[num] = new Collider3D::OBB(this);
				if (collision_[num])
				{
					for (auto& bone : model.bone)
					{
						if (bone.name == BONE_NECK2)
						{
							collision_[num]->SetParentMtx(&model.transMtx, &bone.nowBone);
							break;
						}
					}
					collision_[num]->SetOffsetPosition(COLLISION_OFFSET_POS_NECK2 * s);
					collision_[num]->SetSize(COLLISION_SIZE_NECK2 * s);
				}

				auto& c3 = collision_[static_cast<int>(Collision::WING_L)];
				c3 = new Collider3D::OBB(this);
				if (c3)
				{
					for (auto& bone : model.bone)
					{
						if (bone.name == "WingClaw2_L")
						{
							c3->SetParentMtx(&model.transMtx, &bone.nowBone);
							break;
						}
					}
					c3->SetOffsetPosition(COLLISION_OFFSET_POS_WING_L * s);
					c3->SetOffsetRotation(COLLISION_OFFSET_ROT_WING_L);
					c3->SetSize(COLLISION_SIZE_WING_L * s);
				}
			}
		}
	}
}

/* @fn		Move
 * @brief	移動処理
 * @sa		Update
 * @param	なし
 * @return	なし					*/
void Dragon::Move(void)
{
	CreateFrontVector();

	// 移動向きによりキャラクターの向きを変える
	if ((Abs(velocity_.x) + Abs(velocity_.z) > 0.02f))
	{
		if (animation_ == Animation::WAIT || (animation_ == Animation::WALK || animation_ == Animation::RUN))
		{
			animSpeed_ = 0.5f;
			animation_ = Animation::WALK;
			if (BitCheck(flag_, IS_DASH))
			{
				animSpeed_ = 0.75f;
				animation_ = Animation::RUN;
			}
			mesh_.ChangeAnimation(static_cast<int>(animation_), 30);
		}

		VECTOR3 velocityNorm = VecNorm(velocity_);
		VECTOR3 frontVelocityCross = VecCross(front_, velocityNorm);
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
	else 
	{
		if (animation_ == Animation::WALK || animation_ == Animation::RUN)
		{
			mesh_.ChangeAnimation(static_cast<int>(Animation::WAIT), 30);
			animSpeed_ = 0.75f;
		}
	}

	transform_.position += velocity_;

	velocity_ *= 0.8f;		// 慣性
}

/* @fn		CreateFrontVector
 * @brief	前ベクトルと右ベクトルの生成
 * @sa		Move
 * @param	なし
 * @return	なし					*/
void Dragon::CreateFrontVector(void)
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

	right_ = VecNorm(VecCross(VECTOR3(0, 1, 0), front_));
}

/* @fn		DebugInput
 * @brief	デバッグ用操作
 * @sa		Update
 * @param	なし
 * @return	なし					*/
void Dragon::DebugInput(void)
{
	if (!debugMove_) { return; }

	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return; }

	VECTOR2 inputDir;
	// Input
	inputDir.x = (float)ctrl->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
	inputDir.y = (float)ctrl->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
	// 正規化
	inputDir = VecNorm(inputDir);

	float inputDash = 1;
	BitSub(flag_, IS_DASH);
	if (ctrl->Press(Input::GAMEPAD_R1, DIK_LSHIFT))
	{
		inputDash = 10;
		BitAdd(flag_, IS_DASH);
	}

	VECTOR3 front = (camera_) ? camera_->GetFrontXPlane() : front_;
	VECTOR3 right = (camera_) ? camera_->GetRightXPlane() : right_;

	velocity_ += front * inputDir.y * inputDash * MOVE_SPEED;
	velocity_ -= right * inputDir.x * inputDash * MOVE_SPEED;

	if (currentAttack_) { return; }

	// 咆哮
	if (ctrl->Trigger(Input::GAMEPAD_L2, DIK_T))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::SCREAM)];
		if (currentAttack_) 
		{
			int temp = static_cast<int>(animation_);
			currentAttack_->SetMove(mesh_, animSpeed_, temp);
			animation_ = static_cast<Animation>(temp);
		}
	}

	// 噛みつき
	if (ctrl->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::BITE)];
		if (currentAttack_)
		{
			int temp = static_cast<int>(animation_);
			currentAttack_->SetMove(mesh_, animSpeed_, temp);
			animation_ = static_cast<Animation>(temp);
		}
	}

	// 翼攻撃
	if (ctrl->Trigger(Input::GAMEPAD_SQUARE, DIK_H))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::WING_ATTACK)];
		if (currentAttack_)
		{
			int temp = static_cast<int>(animation_);
			currentAttack_->SetMove(mesh_, animSpeed_, temp);
			animation_ = static_cast<Animation>(temp);
		}
	}

	// 飛行
	if (ctrl->Trigger(Input::GAMEPAD_R2, DIK_G))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::TAKE_OFF)];
		if (currentAttack_)
		{
			bool fly = BitCheck(flag_, IS_FLY);

			int temp = static_cast<int>(animation_);
			static_cast<DragonTakeOff*>(currentAttack_)->SetFly(fly);
			currentAttack_->SetMove(mesh_, animSpeed_, temp);
			animation_ = static_cast<Animation>(temp);

			if(fly)
			{
				BitSub(flag_, IS_FLY);
			}
			else
			{
				BitAdd(flag_, IS_FLY);
			}
		}
	}
}

static VECTOR3 oft  = COLLISION_OFFSET_POS_NECK2;
static VECTOR3 ofr  = VECTOR3(0, 0, 0);
static VECTOR3 size = COLLISION_SIZE_NECK2;

/* @fn		GuiUpdate
 * @brief	Guiの更新処理
 * @param	なし
 * @return	なし					*/
void Dragon::GuiUpdate(void)
{
	//auto& c = collision_[static_cast<int>(Collision::NECK2)];
	//if (c)
	//{
	//	ImGui::DragFloat3("pos", oft);
	//	c->SetOffsetPosition(oft);
	//	ImGui::DragFloat3("rot", ofr);
	//	c->SetOffsetRotation(ofr);
	//	ImGui::DragFloat3("size", size);
	//	c->SetSize(size);
	//}

	// デバッグ用操作の切り替え
	if (ImGui::Button("ctrl"))
	{
		debugMove_ = !debugMove_;
		if (cameraManager_)
		{
			cameraManager_->ChangeCamera((debugMove_) ? 1 : 0);
		}
	}
	ImGui::SameLine();
	ImGui::Text(" : ");
	ImGui::SameLine();
	ImGui::Text((debugMove_) ? "true" : "false");

	ImGui::Text("state : ");
	if (BitCheck(flag_, IS_DASH))
	{
		ImGui::SameLine();
		ImGui::Text("dash ");
	}
	if (BitCheck(flag_, IS_FLY))
	{
		ImGui::SameLine();
		ImGui::Text("fly ");
	}

	if (currentAttack_)
	{
		currentAttack_->GuiUpdate();
	}
}