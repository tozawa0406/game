#include "KnockBackState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "DrawnState/DrawnWaitState.h"

//! @def	回避速度
static constexpr float AVOIDANCE_SPEED = 2.75f;
//! @def	回避アニメーションの終了フレーム
static constexpr int   END_AVOIDANCE_ANIMATION = 30;

/* @brief	コンストラクタ			*/
KnockBackState::KnockBackState(void)
{
}

/* @brief	デストラクタ			*/
KnockBackState::~KnockBackState(void)
{
}

/* @brief	初期化処理
 * @param	(player)	プレイヤーへのポインタ
 * @param	(ctrl)		コントローラへのポインタ
 * @return	なし					*/
void KnockBackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 攻撃中断
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = 1;
	meshAnim.animation = static_cast<int>(Player::Animation::KnockBack);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

	if (auto collider = player_->GetCollider())
	{
		collider->SetEnable(false);
	}
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void KnockBackState::Uninit(void)
{
}

/* @brief	更新処理
 * @param	なし
 * @return	次のステート			*/
PlayerState* KnockBackState::Update(void)
{
	if (!player_) { return nullptr; }

	if (player_->IsEndAnim())
	{
		if (auto collider = player_->GetCollider())
		{
			collider->SetEnable(true);
		}

		if (player_->IsDraw())
		{
			return new DrawnWaitState;
		}
		else
		{
			return new PaidWaitState;
		}
	}

	return nullptr;
}

/* @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	プレイヤーから呼び出される		*/
void KnockBackState::GuiUpdate(void)
{
	ImGui::Text("KnockBack");
	ImGui::Text(string("IsDraw : " + Systems::Instance()->GetDebug()->BoolToString(player_->IsDraw())).c_str());
}
