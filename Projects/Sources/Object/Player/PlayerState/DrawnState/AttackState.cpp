#include "AttackState.h"
#include <random>

#include "DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	アニメーション速度
static constexpr float ANIM_SPEED = 0.6f;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
AttackState::AttackState(void) : next_(false)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
AttackState::~AttackState(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	(player)	プレイヤーへのポインタ
 * @param	(ctrl)		コントローラへのポインタ
 * @return	なし					*/
void AttackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	next_ = false;

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 武器の攻撃を開始
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackStart();
	}
	// 最初の攻撃モーション
	meshAnim.animSpeed = ANIM_SPEED;
	meshAnim.animation = static_cast<int>(Player::Animation::Slash_1);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void AttackState::Uninit(void)
{
}

/* @fn		Update
 * @breif	更新処理
 * @param	なし
 * @return	次のステート			*/
PlayerState* AttackState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	// アニメーションの情報
	int animMax = meshAnim.mesh.GetMaxAnimation();
	int pattern = static_cast<int>(meshAnim.mesh.GetPattern());

	// 終了前に
	if (pattern > (Quarter(animMax) * 3))
	{
		// 次の入力がある
		if (next_)
		{
			// 次の攻撃を行う
			if (const auto& wapon = player_->GetWapon())
			{
				wapon->AttackStart();
			}
			next_ = false;
			// アニメーションが最後まで行ったら最初に戻る
			meshAnim.animation = (meshAnim.animation < static_cast<int>(Player::Animation::Slash_3)) ? static_cast<int>(meshAnim.animation) + 1 : static_cast<int>(Player::Animation::Slash_1);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
		}
	}

	// アニメーションの終了
	if (player_->IsEndAnim())
	{
		if (const auto& wapon = player_->GetWapon())
		{
			wapon->AttackEnd();
		}
		// 抜刀待機状態に戻る
		meshAnim.animation = static_cast<int>(Player::Animation::SetupWait);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);

		return new DrawnWaitState;
	}

	// 攻撃コマンドで次の攻撃
	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U)) { next_ = true; }

	// 回避コマンドで回避ステート
	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		return new AvoidanceState;
	}

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	プレイヤーから呼び出される			*/
void AttackState::GuiUpdate(void)
{
	ImGui::Text("Attack");
}
