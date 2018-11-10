#include "DrawnWaitAction.h"
#include <random>

//! @def	アニメーション変更速度
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;
//! @def	アニメーションの速度
static constexpr float ANIMATION_DEFAULT = 0.75f;

DrawnWaitAction::DrawnWaitAction(void)
{
}

DrawnWaitAction::~DrawnWaitAction(void)
{
}

void DrawnWaitAction::Init(PlayerHunter* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerAction::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 納刀状態と抜刀状態でアニメーションの切り替え
	meshAnim.animation = static_cast<int>(PlayerMove::Animation::SetupWait);
	// 再生速度の設定
	meshAnim.animSpeed = ANIMATION_DEFAULT;

	// アニメーションの変更
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30, true);
}

void DrawnWaitAction::Uninit(void)
{
}

PlayerAction* DrawnWaitAction::Update(void)
{
	return nullptr;
}
