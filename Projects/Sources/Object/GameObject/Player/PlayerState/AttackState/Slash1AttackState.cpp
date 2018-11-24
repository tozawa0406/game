#include "Slash1AttackState.h"
#include <random>

#include "Slash2AttackState.h"
#include "../DrawnState/DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	アニメーション速度
static constexpr float ANIM_SPEED = 0.6f;
//! @def	1撃目のコリジョン開始
static constexpr int COLLISION_START = 18;
//! @def	1撃目のコリジョン終了
static constexpr int COLLISION_END = 26;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
Slash1AttackState::Slash1AttackState(void) : next_(false), debug_nextFrame_(false)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
Slash1AttackState::~Slash1AttackState(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	(player)	プレイヤーへのポインタ
 * @param	(ctrl)		コントローラへのポインタ
 * @return	なし					*/
void Slash1AttackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	next_ = false;

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 最初の攻撃モーション
	meshAnim.animSpeed = ANIM_SPEED;
	meshAnim.animation = static_cast<int>(Player::Animation::Slash_1);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void Slash1AttackState::Uninit(void)
{
}

/* @fn		Update
 * @breif	更新処理
 * @param	なし
 * @return	次のステート			*/
PlayerState* Slash1AttackState::Update(void)
{
	if (!player_) { return nullptr; }

	Wapon* wapon = player_->GetWapon();
	if (!wapon) { return nullptr; }

	auto& meshAnim = player_->GetMeshAnimation();

	// アニメーションの情報
	int animMax   = meshAnim.mesh.GetMaxAnimation();
	float pattern = meshAnim.mesh.GetPattern();

	// 終了前に
	if (pattern > (Quarter(animMax) * 3))
	{
		// 次の入力がある
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

	if (pattern > COLLISION_START)
	{
		if (!wapon->IsAttack())
		{
			// 武器の攻撃を開始
			wapon->AttackStart();
		}
	}
	if (pattern > COLLISION_END)
	{
		// 武器の攻撃を終了
		wapon->AttackEnd();
	}

	// アニメーションの終了
	if (player_->IsEndAnim())
	{
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
 * @detail	プレイヤーから呼び出される			*/
void Slash1AttackState::GuiUpdate(void)
{
	ImGui::Text("Slash1Attack");

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
