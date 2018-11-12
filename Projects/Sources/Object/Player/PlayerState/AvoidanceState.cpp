#include "AvoidanceState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "PaidState/PaidMoveState.h"
#include "DrawnState/DrawnWaitState.h"
#include "DrawnState/DrawnMoveState.h"

//! @def	��𑬓x
static constexpr float AVOIDANCE_SPEED = 2.75f;
//! @def	����A�j���[�V�����̏I���t���[��
static constexpr int   END_AVOIDANCE_ANIMATION = 30;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
AvoidanceState::AvoidanceState(void) : dir_(VECTOR3(0)), isDraw_(false)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
AvoidanceState::~AvoidanceState(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	(player)	�v���C���[�ւ̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void AvoidanceState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// ���O�̏��
	isDraw_ = (meshAnim.animation >= static_cast<int>(Player::Animation::SetupWait)) ? true: false;

	// �U�����f
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = ANIMATION_DEFAULT;
	meshAnim.animation = static_cast<int>(Player::Animation::Roll);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

	// ���͕����ɉ���A���͂��Ȃ��Ƃ��͑O�ɉ��
	if (ctrl_)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// ���K��
		inputDir = VecNorm(inputDir);

		// ��������
		if (inputDir == 0)
		{
			dir_ -= player->GetFront();
		}
		else
		{
			if (const auto& camera_ = player->GetCamera())
			{
				dir_ = VECTOR3(0);
				dir_ += camera_->GetFrontXPlane() * inputDir.y;
				dir_ -= camera_->GetRightXPlane() * inputDir.x;
				dir_ = VecNorm(dir_);
			}
		}
	}
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void AvoidanceState::Uninit(void)
{
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* AvoidanceState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	//�I����
	if (meshAnim.mesh.GetPattern() >= END_AVOIDANCE_ANIMATION)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// ���K��
		inputDir = VecNorm(inputDir);

		VECTOR3 velocity = player_->GetVelocity();
		velocity *= 0.5f;
		player_->SetVelocity(velocity);

		// ���͂�����Έړ��X�e�[�g
		if (inputDir != 0)
		{
			if (isDraw_) { return new DrawnMoveState; }
			else		 { return new PaidMoveState;  }
		}
		else
		{
			// �[����ԂƔ�����ԂŃA�j���[�V�����̐؂�ւ�
			meshAnim.animation = static_cast<int>((isDraw_) ? Player::Animation::SetupWait : Player::Animation::Wait);
			// �A�j���[�V�����̕ύX
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, false);

			if (isDraw_) { return new DrawnWaitState; }
			else		 { return new PaidWaitState;  }
		}
	}

	// �ړ����x
	VECTOR3 velocity = player_->GetVelocity();
	velocity += dir_ * MOVE_SPEED * AVOIDANCE_SPEED;
	player_->SetVelocity(velocity);

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ăяo�����		*/
void AvoidanceState::GuiUpdate(void)
{
	ImGui::Text("Avoidance");
}
