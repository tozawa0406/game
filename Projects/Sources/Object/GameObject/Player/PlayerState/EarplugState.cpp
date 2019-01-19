#include "EarplugState.h"

#include "PaidState/PaidWaitState.h"
#include "DrawnState/DrawnWaitState.h"

EarplugState::EarplugState(void)
{
}

EarplugState::~EarplugState(void)
{
}

void EarplugState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 攻撃中断
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = 0.6f;
	meshAnim.animation = static_cast<int>(Player::Animation::Earplug);
	// 一旦別のアニメーションにすることで、2連続咆哮を受けてもアニメーションを変更させる
	meshAnim.mesh.ChangeAnimation(static_cast<int>(Player::Animation::Wait), ANIMATION_CHANGE_FRAME15);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
}

void EarplugState::Uninit(void)
{
}

PlayerState* EarplugState::Update(void)
{
	if (!player_) { return nullptr; }

	auto& meshAnim = player_->GetMeshAnimation();
	float pattern = meshAnim.mesh.GetPattern();
	if (pattern > 40)		{ meshAnim.animSpeed = 0.4f; }
	else if (pattern > 10)  { meshAnim.animSpeed = 0.4f; }



	if (player_->IsEndAnim())
	{
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

void EarplugState::GuiUpdate(void)
{
	ImGui::Text("Earplug");
//	ImGui::Text(string("IsDraw : " + Systems::Instance()->GetDebug()->BoolToString(player_->IsDraw())).c_str());
}
