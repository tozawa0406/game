//-----------------------------------------------------------------------------
//
//	Auther : ���V�đ�
//-----------------------------------------------------------------------------
#include "Dragon.h"
#include <FrameWork/Object/ObjectManager.h>
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Graphics/DirectX11/Dx11Wrapper.h>
#include "../../../../Scene/GameScene.h"

#include "Attack/DragonScream.h"
#include "Attack/DragonBite.h"
#include "Attack/DragonWingAttack.h"
#include "Attack/DragonTakeOff.h"
#include "Attack/DragonHit.h"
#include "Attack/DragonRush.h"

//! @def	�傫��
static constexpr float SCALE = 0.9f;
//! @def	���C�t
static constexpr int MAX_LIFE = 2500;

//! @def	��s�t���O
static constexpr uint IS_FLY = 0x0002;

//! @def	�̂̃{�[���̖��O
static const     string BONE_BODY = "Spine2";
static const	 Transform COLLISION_OFFSET_BODY = Transform(VECTOR3(-4, 1.5f, 0), VECTOR3(0), VECTOR3(18, 9, 15));
//! @def	���̃{�[���̖��O
static const     string BONE_HEAD = "Head";
static const	 Transform COLLISION_OFFSET_HEAD = Transform(VECTOR3(5, 3, 0), VECTOR3(0), VECTOR3(6, 6, 6));
//! @def	��1�̃{�[��(�̂��)
static const     string BONE_NECK1 = "Neck2";
static const	 Transform COLLISION_OFFSET_NECK1 = Transform(VECTOR3(1, 2, 0), VECTOR3(0), VECTOR3(8, 8, 10));
//! @def	��2�̃{�[��(�����)
static const     string BONE_NECK2 = "Neck4";
static const	 Transform COLLISION_OFFSET_NECK2 = Transform(VECTOR3(3, 1, 0), VECTOR3(0), VECTOR3(10, 5, 5));
//! @def	���܂̃{�[���̖��O
static const     string BONE_WING_L_CLAW = "WingClaw2_L";
static const	 Transform COLLISION_OFFSET_WING_L_CLAW = Transform(VECTOR3(0, -3, -4), VECTOR3(0, -2, 0), VECTOR3(7, 2, 2));

/* @brief	�R���X�g���N�^			*/
Dragon::Dragon(void) : GameObject(ObjectTag::ENEMY), GUI(Systems::Instance(), this, "dragon")
	, flag_(0)
	, debugMove_(false)
	, currentAttack_(nullptr)
	, moveController_(nullptr)
	, accumulation_(0)
	, cameraManager_(nullptr)
	, camera_(nullptr)
{
	meshAnim_.animation = static_cast<int>(Animation::WAIT1);
	meshAnim_.animSpeed = 0;

	for (auto& c : collision_)
	{
		c = nullptr;
	}

	for (auto& a : attack_)
	{
		a = nullptr;
	}

	life_ = MAX_LIFE;
}

/* @brief	�f�X�g���N�^			*/
Dragon::~Dragon(void)
{
}

/* @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Dragon::Init(void)
{
	transform_.position		= VECTOR3(0, 0, 50);
	transform_.rotation.y   = PI;
	transform_.scale		= VECTOR3(SCALE);

	meshAnim_.animSpeed = 0.75f;

	meshAnim_.mesh.Init(Systems::Instance(), (int)Model::Game::DRAGON, &transform_);

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

	arrayNum = static_cast<int>(AttackPattern::HIT);
	attack_[arrayNum] = new DragonHit;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	arrayNum = static_cast<int>(AttackPattern::RUSH);
	attack_[arrayNum] = new DragonRush;
	if (attack_[arrayNum]) { attack_[arrayNum]->Init(this); }

	moveController_ = new DragonMoveController;
	if (moveController_)
	{
		moveController_->Init(this);
		auto objects = manager_->Get();
		for (auto object : objects)
		{
			if (object->GetTag() == ObjectTag::PLAYER)
			{
				moveController_->SetTarget(static_cast<GameObject*>(object));
				break;
			}
		}
	}

#ifdef _SELF_DEBUG
	// �f�o�b�O�p�A�G���쎞�̃J����
	// �J�����}�l�[�W���[�̎擾
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

	// TPS�J�����̐���
	if (cameraManager_)
	{
		camera_ = cameraManager_->CreateAtObjCamera(this, 1, VECTOR3(0, 25, -30));
	}
#endif

}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Dragon::Uninit(void)
{
	UninitDeletePtr(moveController_);

	for (auto& c : collision_)
	{
		DeletePtr(c);
	}

	for (auto& a : attack_)
	{
		UninitDeletePtr(a);
	}

	// ��������TPS�J�����̌�n��
	if (cameraManager_ && camera_)
	{
		cameraManager_->DestroyObjCamera(camera_);
	}
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Dragon::Update(void)
{
	meshAnim_.mesh.material.diffuse = COLOR(1, 1, 1, 1);
	if (ded_) { return; }

#ifdef _SELF_DEBUG
	if (!DebugInput())
#endif
	{
		if (moveController_)
		{
			int act = (currentAttack_) ? -1 : 0;
			moveController_->Action(act, flag_);
			if (act >= 0)
			{
				currentAttack_ = attack_[act];
				if (currentAttack_)
				{
					currentAttack_->SetMove();
				}
			}
		}
	}

	isEndAnim_ = meshAnim_.mesh.Animation(meshAnim_.animSpeed);

	if (TakenDamage()) { return; }

	if (currentAttack_)
	{
		if (currentAttack_->Update())
		{
			currentAttack_->EndMove();
			currentAttack_ = nullptr;
		}
	}

	if (!BitCheck(flag_, IS_FLY) && meshAnim_.animation != static_cast<int>(Animation::TAKE_OFF))
	{
		transform_.position.y = 0;
	}
	Move();

	for (auto& c : collision_)
	{
		if (c)
		{
			c->Update();
			transform_.position += c->GetBack();
		}
	}
}

/* @brief	�����蔻�萶������
 * @sa		Init
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Dragon::CreateCollision(void)
{
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& renderer = systems->GetGraphics())
		{
			if (Dx11Wrapper* wrapper = static_cast<Dx11Wrapper*>(renderer->GetWrapper()))
			{
				const auto& model = wrapper->GetModel(meshAnim_.mesh.GetModelNum());

				SetCollision(static_cast<int>(Collision::BODY) , BONE_BODY , COLLISION_OFFSET_BODY , model);
				SetCollision(static_cast<int>(Collision::HEAD) , BONE_HEAD , COLLISION_OFFSET_HEAD , model);
				SetCollision(static_cast<int>(Collision::NECK1), BONE_NECK1, COLLISION_OFFSET_NECK1, model);
				SetCollision(static_cast<int>(Collision::NECK2), BONE_NECK2, COLLISION_OFFSET_NECK2, model);
				SetCollision(static_cast<int>(Collision::WING_L), BONE_WING_L_CLAW, COLLISION_OFFSET_WING_L_CLAW, model);
			}
		}
	}
}

/* @brief	�R���W�����̐ݒ�
 * @param	(arrayNum)	��������R���W�����̔z��ԍ�
 * @param	(boneName)	�{�[���̖��O
 * @param	(offset)	�I�t�Z�b�g
 * @param	(model)		���f���f�[�^
 * @return	�Ȃ�					*/
void Dragon::SetCollision(int arrayNum, string boneName, const Transform& offset, const MODEL& model)
{
	collision_[arrayNum] = new Collider3D::OBB(this);
	if (collision_[arrayNum])
	{
		for (auto& bone : model.bone)
		{
			if (bone.name == boneName)
			{
				collision_[arrayNum]->SetParentMtx(&model.transMtx, &bone.nowBone);
				break;
			}
		}

		auto s = transform_.scale;
		collision_[arrayNum]->SetOffsetPosition(offset.position * s);
		collision_[arrayNum]->SetOffsetRotation(offset.rotation * s);
		collision_[arrayNum]->SetSize(offset.scale * s);
	}

}

/* @brief	��_���[�W����
 * @sa		Update()
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
bool Dragon::TakenDamage(void)
{
	// ���S����
	if (life_ <= 0)
	{
		if (currentAttack_)
		{
			currentAttack_->EndMove(); 
			currentAttack_ = nullptr;
		}

		if (moveController_)
		{
			UninitDeletePtr(moveController_);
		}

		if (isEndAnim_)
		{
			meshAnim_.mesh.AnimEndPattern();
			ded_ = true;
			return true;
		}

		meshAnim_.animSpeed = 0.5f;
		meshAnim_.animation = static_cast<int>(Animation::DIE);
		meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 30, true);

		int pattern = static_cast<int>(meshAnim_.mesh.GetPattern());
		if(pattern >= 35)
		{
			meshAnim_.animSpeed = 0.5f;
		}
		else if(pattern >= 20)
		{
			meshAnim_.animSpeed = 0.25f;
		}

		for (auto& c : collision_)
		{
			DeletePtr(c);
		}

		return true;
	}

	if (life_ > Quarter(MAX_LIFE) && accumulation_ >= MAX_LIFE * 0.3f)
	{
		accumulation_ = 0;		
		if (currentAttack_) { currentAttack_->EndMove(); }
		currentAttack_ = attack_[static_cast<int>(AttackPattern::HIT)];
		if (currentAttack_)
		{
			currentAttack_->SetMove();
		}

	}

	return false;
}

/* @brief	�_���[�W����
 * @param	(damage)	�_���[�W
 * @return	�Ȃ�					*/
void Dragon::Hit(int damage)
{
	life_ -= damage; 
	accumulation_ += damage;
}

/* @brief	�f�o�b�O�p����
 * @sa		Update()
 * @param	�Ȃ�
 * @return	���s������true			*/
bool Dragon::DebugInput(void)
{
	if (!debugMove_) { return false; }

	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return false; }

	VECTOR2 inputDir;
	// Input
	inputDir.x = (float)ctrl->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
	inputDir.y = (float)ctrl->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
	// ���K��
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

	Animation tempAnim = static_cast<Animation>(meshAnim_.animation);
	if (inputDir != 0)
	{
		if (tempAnim == Animation::WAIT1 || (tempAnim == Animation::WALK || tempAnim == Animation::RUN))
		{
			meshAnim_.animSpeed = 0.5f;
			meshAnim_.animation = static_cast<int>(Animation::WALK);
			if (BitCheck(flag_, IS_DASH))
			{
				meshAnim_.animSpeed = 0.75f;
				meshAnim_.animation = static_cast<int>(Animation::RUN);
			}
			meshAnim_.mesh.ChangeAnimation(meshAnim_.animation, 30);
		}
	}
	else
	{
		if (!currentAttack_)
		{
			if (tempAnim == Animation::WALK || tempAnim == Animation::RUN)
			{
				meshAnim_.mesh.ChangeAnimation(static_cast<int>(Animation::WAIT1), 30);
				meshAnim_.animSpeed = 0.75f;
			}
		}
	}


	if (currentAttack_) { return true; }

	// ���K
	if (ctrl->Trigger(Input::GAMEPAD_L2, DIK_T))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::SCREAM)];
		if (currentAttack_) 
		{
			currentAttack_->SetMove();
		}
	}

	// ���K
	if (ctrl->Trigger(Input::GAMEPAD_CROSS, DIK_K))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::RUSH)];
		if (currentAttack_)
		{
			currentAttack_->SetMove();
		}
	}

	// ��_��
	if (ctrl->Trigger(Input::GAMEPAD_CIRCLE, DIK_O))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::HIT)];
		if (currentAttack_)
		{
			currentAttack_->SetMove();
		}
	}

	// ���݂�
	if (ctrl->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::BITE)];
		if (currentAttack_)
		{
			currentAttack_->SetMove();
		}
	}

	// ���U��
	if (ctrl->Trigger(Input::GAMEPAD_SQUARE, DIK_H))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::WING_ATTACK)];
		if (currentAttack_)
		{
			currentAttack_->SetMove();
		}
	}

	// ��s
	if (ctrl->Trigger(Input::GAMEPAD_R2, DIK_G))
	{
		currentAttack_ = attack_[static_cast<int>(AttackPattern::TAKE_OFF)];
		if (currentAttack_)
		{
			bool fly = BitCheck(flag_, IS_FLY);

			static_cast<DragonTakeOff*>(currentAttack_)->SetFly(fly);
			currentAttack_->SetMove();

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
	return true;
}

/* @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Dragon::GuiUpdate(void)
{
	ImGui::Text("Life : %d", life_);
	if (life_ > Quarter(MAX_LIFE))
	{
		ImGui::Text("Accu : %d / %.2f", accumulation_, MAX_LIFE * 0.3f);
	}
	else
	{
		ImGui::Text("life < Quarter(MAX_LIFE)");
	}

	// �f�o�b�O�p����̐؂�ւ�
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