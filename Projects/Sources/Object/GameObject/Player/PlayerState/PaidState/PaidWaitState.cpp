#include "PaidWaitState.h"
#include <random>

#include "PaidMoveState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"
#include "HealState.h"

//! @def	�ҋ@���[�V�����̍Đ��҂�����
static constexpr int    ANIMATION_WAIT_FRAME = 180;

PaidWaitState::PaidWaitState(void) : 
	waitCnt_(0)
{
}

PaidWaitState::~PaidWaitState(void)
{
}

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

void PaidWaitState::Uninit(void)
{
}

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

	if (meshAnim.animation == static_cast<int>(Player::Animation::Wait)) { waitCnt_++; }
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
			if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
			{
				return new AvoidanceState;
			}
		}

		// �����R�}���h�Ŕ����X�e�[�g
		if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
		{
			return new SetupState;
		}

		// �A�C�e���g�p
		if (ctrl_->Trigger(Input::GAMEPAD_SQUARE, DIK_H))
		{
			if (!ctrl_->Press(Input::GAMEPAD_L1, DIK_R))
			{
				return new HealState;
			}
		}
	}

	return nullptr;
}

void PaidWaitState::GuiUpdate(void)
{
	ImGui::Text("PaidWait");
}
