#include "PaidWaitState.h"
#include <random>

#include "PaidMoveState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"

//! @def	�ړ����x
static constexpr float MOVE_SPEED = 0.06f;
//! @def	�A�j���[�V�����ύX���x
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;
//! @def	�A�j���[�V�����̑��x
static constexpr float ANIMATION_DEFAULT = 0.75f;
//! @def	�ҋ@���[�V�����̍Đ��҂�����
static constexpr int    ANIMATION_WAIT_FRAME = 180;

PaidWaitState::PaidWaitState(void)
{
}

PaidWaitState::~PaidWaitState(void)
{
}

void PaidWaitState::Init(PlayerHunter* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	waitCnt_ = 0;

	auto& meshAnim = player->GetMeshAnimation();

	// �[����ԂƔ�����ԂŃA�j���[�V�����̐؂�ւ�
	meshAnim.animation = static_cast<int>(PlayerMove::Animation::Wait);
	// �Đ����x�̐ݒ�
	meshAnim.animSpeed = ANIMATION_DEFAULT;

	// �A�j���[�V�����̕ύX
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30, true);
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
		meshAnim.animation = static_cast<int>(PlayerMove::Animation::Wait);
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
		meshAnim.animation = static_cast<int>(PlayerMove::Animation::WaitTime1) + (dev() % 2);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30);
	}

	if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
	{
		return new PaidMoveState;
	}

	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		return new AvoidanceState;
	}

	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		return new SetupState;
	}

	return nullptr;
}
