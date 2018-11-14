#include "DieState.h"
#include <random>

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
DieState::DieState(void)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
DieState::~DieState(void)
{
}

/* @fn		Init
 * @brief	初期化処理
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

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void DieState::Uninit(void)
{
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	次のステート			*/
PlayerState* DieState::Update(void)
{
	if (!player_) { return nullptr; }

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	プレイヤーから呼び出される		*/
void DieState::GuiUpdate(void)
{
	ImGui::Text("Die");
}
