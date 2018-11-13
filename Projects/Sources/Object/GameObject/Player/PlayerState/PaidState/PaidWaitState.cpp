#include "PaidWaitState.h"
#include <random>

#include "PaidMoveState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"

//! @def	�ҋ@���[�V�����̍Đ��҂�����
static constexpr int    ANIMATION_WAIT_FRAME = 180;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
PaidWaitState::PaidWaitState(void) : waitCnt_(0)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
PaidWaitState::~PaidWaitState(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	(player)	�v���C���[�ւ̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void PaidWaitState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	waitCnt_ = 0;

	auto& meshAnim = player->GetMeshAnimation();

	// �[����ԂƔ�����ԂŃA�j���[�V�����̐؂�ւ�
	meshAnim.animation = static_cast<int>(Player::Animation::Wait);
	// �Đ����x�̐ݒ�
	meshAnim.animSpeed = ANIMATION_DEFAULT;

	// �A�j���[�V�����̕ύX
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30, true);
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void PaidWaitState::Uninit(void)
{
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* PaidWaitState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	// ���ɖ߂�
	if (player_->IsEndAnim())
	{
		// �[����ԂƔ�����ԂŃA�j���[�V�����̐؂�ւ�
		meshAnim.animation = static_cast<int>(Player::Animation::Wait);
		// �Đ����x�̐ݒ�
		meshAnim.animSpeed = ANIMATION_DEFAULT;

		// �A�j���[�V�����̕ύX
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30, true);
	}

	waitCnt_++;
	// ��莞�Ԃ𒴂�����ҋ@���[�V�����̍Đ�
	if (waitCnt_ > ANIMATION_WAIT_FRAME)
	{
		waitCnt_ = 0;
		// 2��ނ̂����ǂ��炩
		std::random_device dev;
		meshAnim.animation = static_cast<int>(Player::Animation::WaitTime1) + (dev() % 2);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30);
	}

#ifdef _SELF_DEBUG
	if (!player_->IsDebugCtrl())
#endif
	{
		// �L�[���͂���������ړ��X�e�[�g
		if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
		{
			return new PaidMoveState;
		}

		// ����R�}���h�ŉ���X�e�[�g
		if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
		{
			return new AvoidanceState;
		}

		// �����R�}���h�Ŕ����X�e�[�g
		if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
		{
			return new SetupState;
		}
	}

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ăяo�����		*/
void PaidWaitState::GuiUpdate(void)
{
	ImGui::Text("PaidWait");
}
