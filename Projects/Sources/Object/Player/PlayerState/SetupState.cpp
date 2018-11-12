#include "SetupState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "PaidState/PaidMoveState.h"
#include "DrawnState/DrawnWaitState.h"
#include "DrawnState/DrawnMoveState.h"

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
SetupState::SetupState(void) : isDraw_(false)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
SetupState::~SetupState(void)
{
}

/* @fn		Init
 * @brief	初期化
 * @param	(player)	プレイヤーのポインタ
 * @param	(ctrl)		コントローラへのポインタ
 * @return	なし					*/
void SetupState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 納刀時は逆再生
	if (meshAnim.animation >= static_cast<int>(Player::Animation::SetupWait))
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
	meshAnim.animation = static_cast<int>(Player::Animation::Setup);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void SetupState::Uninit(void)
{
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	次のステート			*/
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

			// 移動キーを押している場合は移動ステートへアニメーションを切り替えながら移行
			if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
			{
				if (isDraw_)
				{
					meshAnim.animation = static_cast<int>(Player::Animation::Walk);
					meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
					return new PaidMoveState;
				}
				else
				{
					meshAnim.animation = static_cast<int>(Player::Animation::SetupWalk);
					meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
					return new DrawnMoveState;
				}
			}
		}
	}

	// キー入力がない場合は待機モーションへ移行
	if (player_->IsEndAnim())
	{
		if (isDraw_) { return new PaidWaitState;  }
		else		 { return new DrawnWaitState; }
	}

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	プレイヤーから呼び出される		*/
void SetupState::GuiUpdate(void)
{
	ImGui::Text("Setup");
}
