#include "DrawnWaitState.h"
#include <random>

#include "DrawnMoveState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"
#include "../AttackState/Slash1AttackState.h"

DrawnWaitState::DrawnWaitState(void)
{
}

DrawnWaitState::~DrawnWaitState(void)
{
}

void DrawnWaitState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 納刀状態と抜刀状態でアニメーションの切り替え
	meshAnim.animation = static_cast<int>(Player::Animation::SetupWait);
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
#ifdef _SELF_DEBUG
	if (!player_->IsDebugCtrl())
#endif
	{
		// 入力で移動ステートへ
		if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
		{
			return new DrawnMoveState;
		}

		// 回避コマンドで回避ステート
		if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
		{
			if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
			{
				return new AvoidanceState;
			}
		}

		// 納刀コマンドで納刀ステート
		if (ctrl_->Trigger(Input::GAMEPAD_SQUARE, DIK_H) || ctrl_->Trigger(Input::GAMEPAD_R1, DIK_H))
		{
			return new SetupState;
		}

		// 攻撃コマンドで攻撃ステート
		if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
		{
			return new Slash1AttackState;
		}
	}
	return nullptr;
}

void DrawnWaitState::GuiUpdate(void)
{
	ImGui::Text("DrawnWait");
}
