#include "DieState.h"
#include <random>

/* @brief	コンストラクタ			*/
DieState::DieState(void)
{
}

/* @brief	デストラクタ			*/
DieState::~DieState(void)
{
}

/* @brief	初期化処理
 * @param	(player)	プレイヤーへのポインタ
 * @param	(ctrl)		コントローラへのポインタ
 * @return	なし					*/
void DieState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 攻撃中断
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = ANIMATION_DEFAULT;
	meshAnim.animation = static_cast<int>(Player::Animation::Die);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

	if (const auto& collider = player->GetCollider())
	{
		collider->SetEnable(false);
	}
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void DieState::Uninit(void)
{
}

/* @brief	更新処理
 * @param	なし
 * @return	次のステート			*/
PlayerState* DieState::Update(void)
{
	if (!player_) { return nullptr; }

	return nullptr;
}

/* @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	プレイヤーから呼ばれる	*/
void DieState::GuiUpdate(void)
{
	ImGui::Text("Die");
}
