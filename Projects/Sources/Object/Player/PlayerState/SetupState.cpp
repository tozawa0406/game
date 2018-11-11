#include "SetupState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "PaidState/PaidMoveState.h"
#include "DrawnState/DrawnWaitState.h"
#include "DrawnState/DrawnMoveState.h"

//! @def	移動速度
static constexpr float MOVE_SPEED = 0.06f;
//! @def	回避速度
static constexpr float AVOIDANCE_SPEED = 2.75f;
//! @def	アニメーション変更速度
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;
//! @def	アニメーションの速度
static constexpr float ANIMATION_DEFAULT = 0.75f;
//! @def	回避アニメーションの終了フレーム
static constexpr int END_AVOIDANCE_ANIMATION = 30;

SetupState::SetupState(void) : isDraw_(false)
{
}

SetupState::~SetupState(void)
{
}

void SetupState::Init(PlayerHunter* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 納刀時は逆再生
	PlayerMove::Animation temp = static_cast<PlayerMove::Animation>(meshAnim.animation);
	if (temp == PlayerMove::Animation::SetupWait || temp == PlayerMove::Animation::SetupWalk) 
	{
		isDraw_ = true;
		meshAnim.animSpeed = -0.75f; 
	}
	else
	{
		isDraw_ = false;
		meshAnim.animSpeed = 0.75f;
	}

	// アニメーション
	meshAnim.animation = static_cast<int>(PlayerMove::Animation::Setup);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
}

void SetupState::Uninit(void)
{
}

PlayerState* SetupState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	// 納刀抜刀中であり、アニメーションが一定以下
	if (fabs(meshAnim.mesh.GetPattern()) >= 30)
	{
		// 武器の状態切り替え
		if (const auto& wapon = player_->GetWapon())
		{
			wapon->Setup(isDraw_);
			if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
			{
				if (isDraw_)
				{
					meshAnim.animation = static_cast<int>(PlayerMove::Animation::Walk);
					meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
					return new PaidMoveState;
				}
				else
				{
					meshAnim.animation = static_cast<int>(PlayerMove::Animation::SetupWalk);
					meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
					return new DrawnMoveState;
				}
			}
		}
	}

	if (player_->IsEndAnim())
	{
		if (isDraw_) { return new PaidWaitState;  }
		else		 { return new DrawnWaitState; }
	}

	return nullptr;
}
