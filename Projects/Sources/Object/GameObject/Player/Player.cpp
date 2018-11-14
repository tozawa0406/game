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

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
Player::Player(void) : GameObject(Object::Tag::PLAYER), GUI(Systems::Instance(), this, "player")
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

/* @fn		�f�X�g���N�^
 * @brief	...						*/
Player::~Player(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���b�V���ⓖ���蔻��Ȃǂ̏�����		*/
void Player::Init(void)
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

	state_ = new PaidWaitState;
	if (state_)
	{
		state_->Init(this, GetCtrl(0));
	}
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Player::Uninit(void)
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
void Player::Update(void)
{
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
	meshAnim_.mesh.material.diffuse = c;
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

	Move();

	OnGround();
}

void Player::Hit(int damage)
{
	if (collider_->IsEnable())
	{
		life_ -= damage;
		UninitDeletePtr(state_);
		state_ = new KnockBackState;
		state_->Init(this, GetCtrl(0));
	}
}

/* @fn		GuiUpdate
 * @brief	�f�o�b�O�p�`��X�V
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Player::GuiUpdate(void)
{
	ImGui::Text("Life : %d", life_);

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