#include "SetupAttackState.h"
#include <random>

#include "Slash2AttackState.h"
#include "../DrawnState/DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	当たり判定開始
static constexpr float COLLISION_START = 30;
//! @def	当たり判定終了
static constexpr float COLLISION_END = 45;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
SetupAttackState::SetupAttackState(void) : isDraw_(false), next_(false), debug_nextFrame_(0)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
SetupAttackState::~SetupAttackState(void)
{
}

/* @fn		Init
 * @brief	初期化
 * @param	(player)	プレイヤーのポインタ
 * @param	(ctrl)		コントローラへのポインタ
 * @return	なし					*/
void SetupAttackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	isDraw_ = false;

	meshAnim.animSpeed = 0.75f;
	meshAnim.animation = static_cast<int>(Player::Animation::SetupDrawn);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void SetupAttackState::Uninit(void)
{
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	次のステート			*/
PlayerState* SetupAttackState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	float pattern = meshAnim.mesh.GetPattern();
	// 納刀抜刀中であり、アニメーションが一定以下
	if (pattern >= COLLISION_START)
	{
		// 武器の状態切り替え
		if (const auto& wapon = player_->GetWapon())
		{
			if (isDraw_ == player_->IsDraw())
			{
				wapon->Setup(isDraw_);
				player_->SetDraw(!player_->IsDraw());
				wapon->AttackStart();
			}
		}
	}

	if (pattern > COLLISION_END)
	{
		if (const auto& wapon = player_->GetWapon())
		{
			wapon->AttackEnd();
		}
	}

	// アニメーションの情報
	int animMax = meshAnim.mesh.GetMaxAnimation();

	// 終了前に
	if (pattern > (Quarter(animMax) * 3.0f))
	{
		if (next_)
		{
			// 次の攻撃を行う
			next_ = false;
			// アニメーションが最後まで行ったら最初に戻る
			meshAnim.animation = static_cast<int>(Player::Animation::Slash_2);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

			return new Slash2AttackState;
		}
	}

	// 攻撃コマンドで次の攻撃
	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U)) { next_ = true; }

	// キー入力がない場合は待機モーションへ移行
	if (player_->IsEndAnim())
	{
		// 抜刀待機状態に戻る
		meshAnim.mesh.AnimEndPattern();
		meshAnim.animation = static_cast<int>(Player::Animation::SetupWait);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);
		return new DrawnWaitState;
	}

	// 回避コマンドで回避ステート
	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
		{
			return new AvoidanceState;
		}
	}

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	プレイヤーから呼び出される		*/
void SetupAttackState::GuiUpdate(void)
{
	ImGui::Text("SetupAttack");

	if (const auto& systems = Systems::Instance())
	{
		if (const auto& debug = systems->GetDebug())
		{
			if (debug->GetDebugPause())
			{
				if (ImGui::Button("Next Frame"))
				{
					debug_nextFrame_ = true;
					debug->SetDebugPause(false);
				}
				ImGui::SameLine();
				ImGui::Text("pattern : %.2f", player_->GetMeshAnimation().mesh.GetPattern());
			}
			else
			{
				if (debug_nextFrame_)
				{
					debug->SetDebugPause(true);
				}
				debug_nextFrame_ = false;
			}
		}
	}
}
