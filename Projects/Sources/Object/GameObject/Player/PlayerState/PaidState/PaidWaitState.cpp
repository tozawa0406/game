#include "PaidWaitState.h"
#include <random>

#include "PaidMoveState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"
#include "HealState.h"

//! @def	待機モーションの再生待ち時間
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

	// 納刀状態と抜刀状態でアニメーションの切り替え
	meshAnim.animation = static_cast<int>(Player::Animation::Wait);
	// 再生速度の設定
	meshAnim.animSpeed = ANIMATION_DEFAULT;

	// アニメーションの変更
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30, true);
}

void PaidWaitState::Uninit(void)
{
}

PlayerState* PaidWaitState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	// 元に戻る
	if (player_->IsEndAnim())
	{
		// 納刀状態と抜刀状態でアニメーションの切り替え
		meshAnim.animation = static_cast<int>(Player::Animation::Wait);
		// 再生速度の設定
		meshAnim.animSpeed = ANIMATION_DEFAULT;

		// アニメーションの変更
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30, true);
	}

	if (meshAnim.animation == static_cast<int>(Player::Animation::Wait)) { waitCnt_++; }
	// 一定時間を超えたら待機モーションの再生
	if (waitCnt_ > ANIMATION_WAIT_FRAME)
	{
		waitCnt_ = 0;
		// 2種類のうちどちらか
		std::random_device dev;
		meshAnim.animation = static_cast<int>(Player::Animation::WaitTime1) + (dev() % 2);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30);
	}

#ifdef _SELF_DEBUG
	if (!player_->IsDebugCtrl())
#endif
	{
		// キー入力があったら移動ステート
		if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
		{
			return new PaidMoveState;
		}

		// 回避コマンドで回避ステート
		if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
		{
			if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
			{
				return new AvoidanceState;
			}
		}

		// 抜刀コマンドで抜刀ステート
		if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
		{
			return new SetupState;
		}

		// アイテム使用
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
