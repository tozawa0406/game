#include "PlayerMove.h"
#include <FrameWork/Systems/Input/Controller.h>
#include "../../Scene/EachScene/GameScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Object/ObjectManager.h>
#include <random>

#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>

//! @def	�ړ����x
static constexpr float MOVE_SPEED = 0.06f;
//! @def	�傫��
static constexpr float SCALE = 0.1f;

//! @def	�̂̃{�[���̖��O
static const     string BONE_BODY_NAME = "Character1_Hips";
//! @def	�E��̃{�[���̖��O
static const     string BONE_RIGHT_HAND_NAME = "Character1_RightHand";
//! @def	�����蔻��̃I�t�Z�b�g�ʒu
static const     VECTOR3 COLLISION_OFFSET = VECTOR3(0, 7.5f, 0);
//! @def	�����蔻��̃T�C�Y
static const     VECTOR3 COLLISION_SIZE = VECTOR3(3, 15, 3);

//! @def	�A�j���[�V�����ύX���x
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;

//! @def	�ҋ@���[�V�����̍Đ��҂�����
static constexpr int    ANIMATION_WAIT_FRAME = 180;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
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

/* @fn		�f�X�g���N�^
 * @brief	...						*/
PlayerMove::~PlayerMove(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���b�V���ⓖ���蔻��Ȃǂ̏�����		*/
void PlayerMove::Init(void)
{
	const auto& systems = Systems::Instance();
	if (!systems) { return; }

	// ���b�V��
	mesh_.Init(systems, static_cast<int>(Model::Game::UNITY_CHAN), &transform_);
	mesh_.shader = Shader::ENUM::DEFAULT;

	// �X�P�[���̐ݒ�
	transform_.scale = VECTOR3(SCALE);

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
		camera_ = cameraManager_->CreateAtObjCamera(this, 0);
	}


	if (const auto& renderer = systems->GetRenderer())
	{
		if (DirectX11Wrapper* wrapper = static_cast<DirectX11Wrapper*>(renderer->GetWrapper()))
		{
			// ����
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

	// �����蔻��
	collider_ = new Collider3D::OBB(this);
	collider_->SetOffset(COLLISION_OFFSET);
	collider_->SetSize(COLLISION_SIZE);
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void PlayerMove::Uninit(void)
{
	// ��������TPS�J�����̌�n��
	if (cameraManager_ && camera_)
	{
		cameraManager_->DestroyObjCamera(camera_);
	}
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
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
 * @brief	�ҋ@���[�V����
 * @sa		Update()
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void PlayerMove::StandBy(void)
{
	// ���ɖ߂�
	if (((animation_ == Animation::WaitTime1) || (animation_ == Animation::WaitTime2)) || 
		((animation_ == Animation::Setup)     || (animation_ == Animation::Roll)) && BitCheck(flag_, IS_END_ANIMATION))
	{
		// �[����ԂƔ�����ԂŃA�j���[�V�����̐؂�ւ�
		animation_ = (BitCheck(flag_, IS_DRAWN)) ? Animation::SetupWait : Animation::Wait;
		// �Đ����x�̐ݒ�
		animCnt_ = ANIMATION_DEFAULT;

		// �A�j���[�V�����̕ύX
		mesh_.ChangeAnimation(static_cast<int>(animation_), ANIMATION_CHANGE_FRAME30, true);
	}

	// �������͑ҋ@���[�V�����Ȃ�
	if (BitCheck(flag_, IS_DRAWN)) { return; }

	// �[�����̑ҋ@��
	if (animation_ == Animation::Wait)
	{
		waitTime_++;
		// ��莞�Ԃ𒴂�����ҋ@���[�V�����̍Đ�
		if (waitTime_ > ANIMATION_WAIT_FRAME)
		{
			// 2��ނ̂����ǂ��炩
			std::random_device dev;
			animation_ = (Animation)(static_cast<int>(Animation::WaitTime1) + (dev() % 2));
			mesh_.ChangeAnimation(static_cast<int>(animation_), ANIMATION_CHANGE_FRAME30);
		}
	}
	else 
	{
		// �ҋ@���Ԃ̏�����
		waitTime_ = 0; 
	}
}

/* @fn		Move
 * @brief	�ړ�����
 * @sa		Update()
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���͌��m�A�A�j���[�V�����؂�ւ��A
			�O�x�N�g���̐����A�����̕ύX�A�Փ˔���		*/
void PlayerMove::Move(void)
{
	// Input
	inputDir_.x = (float)GetCtrl(0)->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
	inputDir_.y = (float)GetCtrl(0)->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
	// ���K��
	inputDir_ = VecNorm(inputDir_);

	inputDash_  = (GetCtrl(0)->Press(Input::GAMEPAD_R1, DIK_LSHIFT)) ? 2.0f : 1.0f;;

	// �[���������͈ړ�����
	if (!BitCheck(flag_, IS_AVOIDANCE | IS_SETUP | IS_ATTACK))
	{
		// ���蔻��
		if (BitCheck(flag_, IS_DRAWN)) { inputDash_ = 0.75f; }
		inputDir_ *= inputDash_;

		// �A�j���[�V�����؂�ւ�
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
		// �ړ����x
		if (camera_)
		{
			velocity_ += camera_->GetFrontXPlane() * inputDir_.y * MOVE_SPEED;
			velocity_ -= camera_->GetRightXPlane() * inputDir_.x * MOVE_SPEED;
		}
	}

	// �L�����N�^�[�̑O�x�N�g���̐���
	CreateFrontVector();

	if (BitCheck(flag_, IS_AVOIDANCE))
	{
		velocity_ += avoidanceDir_ * MOVE_SPEED * 2.25f;
	}

	// �ړ������ɂ��L�����N�^�[�̌�����ς���
	if ((Abs(velocity_.x) + Abs(velocity_.z) > 0.02f))
	{
		VECTOR3 velocityNorm	= VecNorm(velocity_);
		VECTOR3 frontVelocityCross		= VecCross(front_, velocityNorm);
		float	dot = VecDot(front_, velocityNorm);

		// �O�����ɐi�݂����Ƃ�
		int sign = 1;
		if (frontVelocityCross.y < 0) { sign = -1; }

		int upY = (int)(((frontVelocityCross.y * 10) + (5 * sign)) * 0.1f);
		// ���ς�0�ȉ��̎�(���ɐi�ނƂ�)
		if (upY == 0 && dot == 1)
		{
			// �����I�ɉ�
			frontVelocityCross.y = 1.0f * sign;
		}

		transform_.rotation.y += frontVelocityCross.y * 0.3f;
	}
	else { velocity_ = VECTOR3(0); }

	transform_.position += velocity_;		// �ړ�

	const auto& hits = collider_->Hit();
	for (auto& hit : hits)
	{
		if (hit->GetTag() == Object::Tag::STATIC)
		{
			this->transform_.position -= velocity_;
		}
	}

	velocity_ *= 0.8f;						// ����
}

/* @fn		CreateFrontVector
 * @brief	�O�x�N�g���̐���
 * @sa		Move()
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
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
 * @brief	�ڒn����
 * @sa		Update()
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
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
 * @brief	�f�o�b�O�p�`��X�V
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
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
