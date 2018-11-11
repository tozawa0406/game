#include "PlayerMove.h"
#include <FrameWork/Systems/Input/Controller.h>
#include "../../Scene/GameScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Object/ObjectManager.h>
#include <random>

#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>
#include "PlayerState/PlayerState.h"

//! @def	�ړ����x
static constexpr float MOVE_SPEED = 0.06f;
//! @def	�傫��
static constexpr float SCALE = 0.1f;

//! @def	�̂̃{�[���̖��O
static const     string BONE_BODY_NAME = "Character1_Hips";
//! @def	�E��̃{�[���̖��O
static const     string BONE_RIGHT_HAND_NAME = "Character1_RightHand";
//! @def	�����蔻��̃I�t�Z�b�g�ʒu
static const     VECTOR3 COLLISION_OFFSET_POS = VECTOR3(0, 7.5f, 0);
//! @def	�����蔻��̃T�C�Y
static const     VECTOR3 COLLISION_SIZE = VECTOR3(3, 15, 3);

//! @def	�A�j���[�V�����ύX���x
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;

//! @def	�ҋ@���[�V�����̍Đ��҂�����
static constexpr int    ANIMATION_WAIT_FRAME = 180;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
PlayerMove::PlayerMove(void) : Object(Object::Tag::PLAYER), GUI(Systems::Instance(), this, "player")
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
{
	meshAnim_.animation = static_cast<int>(Animation::Wait);
	meshAnim_.animSpeed = ANIMATION_DEFAULT;
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
	meshAnim_.mesh.Init(systems, static_cast<int>(Model::Game::UNITY_CHAN), &transform_);
	meshAnim_.mesh.shader = Shader::ENUM::DEFAULT;

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

	// �����蔻��
	collider_ = new Collider3D::OBB(this);
	collider_->SetOffsetPosition(COLLISION_OFFSET_POS);
	collider_->SetSize(COLLISION_SIZE);
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void PlayerMove::Uninit(void)
{
	DeletePtr(collider_);
	// ��������TPS�J�����̌�n��
	if (cameraManager_ && camera_)
	{
		cameraManager_->DestroyObjCamera(camera_);
	}

	DeletePtr(state_);
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void PlayerMove::Update(void)
{
	isEndAnim_ = meshAnim_.mesh.Animation(meshAnim_.animSpeed);

#ifdef _SELF_DEBUG
	// �f�o�b�O�p�A�G�̑��쒆�Ƀv���C���[�𑀍삵�Ȃ�
	if (cameraManager_ && cameraManager_->GetMainNum() != 0) { return; }
#endif

	if (state_)
	{
		const auto& temp = state_->Update();
		if (temp)
		{
			UninitDeletePtr(state_);
			temp->Init(static_cast<PlayerHunter*>(this), GetCtrl(0));
			state_ = temp;
		}
	}

	Move();

	OnGround();
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
	// �L�����N�^�[�̑O�x�N�g���̐���
	CreateFrontVector();

	// �ړ������ɂ��L�����N�^�[�̌�����ς���
	if ((Abs(velocity_.x) + Abs(velocity_.z) > 0.02f))
	{
		VECTOR3 velocityNorm		= VecNorm(velocity_);
		VECTOR3 frontVelocityCross	= VecCross(front_, velocityNorm);
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

	if (BitCheck(flag_, IS_DRAWN))
	{
		ImGui::SameLine();
		ImGui::Text("drawn ");
	}
	if (BitCheck(flag_, IS_SETUP))
	{
		ImGui::SameLine();
		ImGui::Text("setup ");
	}
	if (BitCheck(flag_, IS_AVOIDANCE))
	{
		ImGui::SameLine();
		ImGui::Text("avoidance ");
	}
	if (BitCheck(flag_, IS_ATTACK))
	{
		ImGui::SameLine();
		ImGui::Text("attack ");
	}

	ImGui::Text("front : %.2f, %.2f, %.2f", front_.x, front_.y, front_.z);
	ImGui::Text("veloc : %.2f, %.2f, %.2f", velocity_.x, velocity_.y, velocity_.z);

	ImGui::Text("animation    : %d", meshAnim_.mesh.GetAnimation());
	ImGui::Text("animationOld : %d", meshAnim_.mesh.GetAnimationOld());

	ImGui::Text("pattern      : %.2f", meshAnim_.mesh.GetPattern());
}
