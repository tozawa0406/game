#include "DrawnWaitState.h"
#include <random>

#include "DrawnMoveState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"

//! @def	アニメーション変更速度
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;
//! @def	アニメーションの速度
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

	// 納刀状態と抜刀状態でアニメーションの切り替え
	meshAnim.animation = static_cast<int>(PlayerMove::Animation::SetupWait);
	// 再生速度の設定
	meshAnim.animSpeed = ANIMATION_DEFAULT;

	// アニメーションの変更
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
