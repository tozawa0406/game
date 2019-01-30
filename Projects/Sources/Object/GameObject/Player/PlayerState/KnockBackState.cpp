#include "KnockBackState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "DrawnState/DrawnWaitState.h"

//! @def	回避速度
static constexpr float AVOIDANCE_SPEED = 2.75f;
//! @def	回避アニメーションの終了フレーム
static constexpr int   END_AVOIDANCE_ANIMATION = 30;

KnockBackState::KnockBackState(void)
{
}

KnockBackState::~KnockBackState(void)
{
}

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

	if (auto collider = player_->GetDefenseCollider())
	{
		collider->SetEnable(false);
	}
}

void KnockBackState::Uninit(void)
{
}

PlayerState* KnockBackState::Update(void)
{
	if (!player_) { return nullptr; }

	if (player_->IsEndAnim())
	{
		if (auto collider = player_->GetDefenseCollider())
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

void KnockBackState::GuiUpdate(void)
{
	ImGui::Text("KnockBack");
	ImGui::Text(string("IsDraw : " + Systems::Instance()->GetDebug()->BoolToString(player_->IsDraw())).c_str());
}
