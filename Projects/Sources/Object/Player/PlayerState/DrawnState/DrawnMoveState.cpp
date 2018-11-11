#include "DrawnMoveState.h"

#include "DrawnWaitState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"

//! @def	�ړ����x
static constexpr float MOVE_SPEED = 0.06f;
//! @def	�A�j���[�V�����ύX���x
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;

DrawnMoveState::DrawnMoveState(void)
{
}

DrawnMoveState::~DrawnMoveState(void)
{
}

void DrawnMoveState::Init(PlayerHunter* player, Controller* ctrl)
{
	PlayerState::Init(player, ctrl);

	inputDir_  = VECTOR2(0);
}

void DrawnMoveState::Uninit(void)
{
}

PlayerState* DrawnMoveState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	if (ctrl_)
	{
		// Input
		inputDir_.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir_.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// ���K��
		inputDir_ = VecNorm(inputDir_);
	}

	inputDir_ *= 0.75f;

	// �A�j���[�V�����؂�ւ�
	if (fabs(inputDir_.x) + fabs(inputDir_.y) > 0)
	{
		meshAnim.animSpeed = 0.55f;
		meshAnim.animation = static_cast<int>(PlayerMove::Animation::SetupWalk);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30);
	}
	else
	{
		return new DrawnWaitState;;
	}

	// �ړ����x
	if (const auto& camera = player_->GetCamera())
	{
		VECTOR3 velocity = player_->GetVelocity();
		velocity += camera->GetFrontXPlane() * inputDir_.y * MOVE_SPEED;
		velocity -= camera->GetRightXPlane() * inputDir_.x * MOVE_SPEED;
		player_->SetVelocity(velocity);
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
