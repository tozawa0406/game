#include "DrawnWaitState.h"
#include <random>

#include "DrawnMoveState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"
#include "../AttackState/Slash1AttackState.h"

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
DrawnWaitState::DrawnWaitState(void)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
DrawnWaitState::~DrawnWaitState(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	(player)	プレイヤーへのポインタ
 * @param	(ctrl)		コントローラへのポインタ
 * @return	なし					*/
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

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void DrawnWaitState::Uninit(void)
{
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	次のステートへ			*/
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

/* @fn		GuiUpdate
 * @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	プレイヤーから呼び出される		*/
void DrawnWaitState::GuiUpdate(void)
{
	ImGui::Text("DrawnWait");
}
