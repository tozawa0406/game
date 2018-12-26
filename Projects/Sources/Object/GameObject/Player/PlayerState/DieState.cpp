#include "DieState.h"
#include <random>

DieState::DieState(void)
{
}

DieState::~DieState(void)
{
}

void DieState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// UŒ‚’†’f
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

void DieState::Uninit(void)
{
}

PlayerState* DieState::Update(void)
{
	if (!player_) { return nullptr; }

	return nullptr;
}

void DieState::GuiUpdate(void)
{
	ImGui::Text("Die");
}
