#include "DrawnWaitState.h"
#include <random>

#include "DrawnMoveState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"

//! @def	�A�j���[�V�����ύX���x
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;
//! @def	�A�j���[�V�����̑��x
static constexpr float ANIMATION_DEFAULT = 0.75f;

DrawnWaitState::DrawnWaitState(void)
{
}

DrawnWaitState::~DrawnWaitState(void)
{
}

void DrawnWaitState::Init(PlayerHunter* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �[����ԂƔ�����ԂŃA�j���[�V�����̐؂�ւ�
	meshAnim.animation = static_cast<int>(PlayerMove::Animation::SetupWait);
	// �Đ����x�̐ݒ�
	meshAnim.animSpeed = ANIMATION_DEFAULT;

	// �A�j���[�V�����̕ύX
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30, true);
}

void DrawnWaitState::Uninit(void)
{
}

PlayerState* DrawnWaitState::Update(void)
{
	if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
	{
		return new DrawnMoveState;
	}

	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		return new AvoidanceState;
	}

	if (ctrl_->Trigger(Input::GAMEPAD_SQUARE, DIK_H))
	{
		return new SetupState;
	}

	return nullptr;
}
