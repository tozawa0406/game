#include "AttackState.h"
#include <random>

#include "DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	アニメーション速度
static constexpr float ANIM_SPEED = 0.6f;

//! @def	1撃目のコリジョン開始
static constexpr int COLLISION_SLASH1_START = 30;
//! @def	2撃目のコリジョン開始
static constexpr int COLLISION_SLASH2_START = 35;
//! @def	3撃目のコリジョン開始
static constexpr int COLLISION_SLASH3_START = 70;

//! @def	1撃目のコリジョン終了
static constexpr int COLLISION_SLASH1_END = 43;
//! @def	2撃目のコリジョン終了
static constexpr int COLLISION_SLASH2_END = 58;
//! @def	3撃目のコリジョン終了
static constexpr int COLLISION_SLASH3_END = 110;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
AttackState::AttackState(void) : frame_(0), next_(false), debug_nextFrame_(false)
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

	Wapon* wapon = player_->GetWapon();
	if (!wapon) { return nullptr; }

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
			frame_ = 0;
			next_ = false;
			// アニメーションが最後まで行ったら最初に戻る
			meshAnim.animation = (meshAnim.animation < static_cast<int>(Player::Animation::Slash_3)) ? static_cast<int>(meshAnim.animation) + 1 : static_cast<int>(Player::Animation::Slash_1);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
		}
	}

	frame_++;
	int start[] = { COLLISION_SLASH1_START, COLLISION_SLASH2_START, COLLISION_SLASH3_START };
	int end[] = { COLLISION_SLASH1_END, COLLISION_SLASH2_END, COLLISION_SLASH3_END };

	int num = ((sizeof(start) / sizeof(start[0])) - 1) - (static_cast<int>(Player::Animation::Slash_3) - meshAnim.animation);

	if (frame_ > start[num])
	{
		if (!wapon->IsAttack())
		{
			// 武器の攻撃を開始
			wapon->AttackStart();
		}
	}
	if (frame_ > end[num])
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
void AttackState::GuiUpdate(void)
{
	ImGui::Text("Attack");

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
				ImGui::Text("frame : %d", frame_);
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
