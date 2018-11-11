#include "PaidMoveState.h"

#include "PaidWaitState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"

//! @def	移動速度
static constexpr float MOVE_SPEED = 0.06f;
//! @def	アニメーション変更速度
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;

PaidMoveState::PaidMoveState(void)
{
}

PaidMoveState::~PaidMoveState(void)
{
}

void PaidMoveState::Init(PlayerHunter* player, Controller* ctrl)
{
	PlayerState::Init(player, ctrl);

	inputDir_  = VECTOR2(0);
	inputDash_ = 0;
}

void PaidMoveState::Uninit(void)
{
}

PlayerState* PaidMoveState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	if (ctrl_)
	{
		// Input
		inputDir_.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir_.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// 正規化
		inputDir_ = VecNorm(inputDir_);

		inputDash_ = (ctrl_->Press(Input::GAMEPAD_R1, DIK_LSHIFT)) ? 2.5f : 1.0f;
	}


	inputDir_ *= inputDash_;

	// アニメーション切り替え
	if (fabs(inputDir_.x) + fabs(inputDir_.y) > 0)
	{
		meshAnim.animSpeed   = 0.55f;
		meshAnim.animation = static_cast<int>((inputDash_ <= 1) ? PlayerMove::Animation::Walk : PlayerMove::Animation::Run);
		int cnt = (meshAnim.animation == static_cast<int>(PlayerMove::Animation::Run)) ? 15 : ANIMATION_CHANGE_FRAME30;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, cnt);
	}
	else
	{
		return new PaidWaitState;
	}

	// 移動速度
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

	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		return new SetupState;
	}

	return nullptr;
}
