#include "HealState.h"

#include "PaidWaitState.h"

#include <FrameWork/Systems/Particle/Particle.h>
#include <random>

HealState::HealState(void) : 
	heal_(false)
	, itemId_(ItemID::UNKNOWN)
{
}

HealState::~HealState(void)
{
}

void HealState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 納刀状態と抜刀状態でアニメーションの切り替え
	meshAnim.animation = static_cast<int>(Player::Animation::Heal);
	// 再生速度の設定
	meshAnim.animSpeed = ANIMATION_DEFAULT;

	// アニメーションの変更
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);

	if (const auto& itemList = player->GetItemLIst())
	{
		itemId_ = itemList->GetCurrentItem().itemID;
		itemList->UseItem();
	}
}

void HealState::Uninit(void)
{
}

PlayerState* HealState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	const auto& pattern = meshAnim.mesh.GetPattern();
	if (40 < pattern && pattern < 85)
	{
		COLOR c = COLOR(1, 1, 1);
		if (itemId_ == ItemID::Recovery)		{ c = COLOR(0, 1, 0); }
		else if (itemId_ == ItemID::Rations)	{ c = COLOR(1, 1, 0); }
		if (!heal_) 
		{
			heal_ = true;
			if (itemId_ == ItemID::Recovery)	 { player_->Recovery(30); }
			else if (itemId_ == ItemID::Rations) { player_->AddMaxStamina(); }
		}

		if (const auto& systems = Systems::Instance())
		{
			VECTOR2 v = VecCircle(360) * 0.1f;
			auto p = player_->GetTransform().position;
			p.y += 10;
			std::random_device rdev;
			float r = (rdev() % 10) * 0.15f;
			PARTICLE_DATA data = PARTICLE_DATA(p, r, 30, c, VECTOR3(v.x, v.y, v.x));
			const auto& particle = new Particle(systems->GetParticleManager(), data);
			particle->SetTexture(static_cast<int>(Resources::Texture::Camp::EFFECT));
		}
	}

	// 元に戻る
	if (player_->IsEndAnim())
	{
		// 納刀状態と抜刀状態でアニメーションの切り替え
		meshAnim.animation = static_cast<int>(Player::Animation::Wait);
		// 再生速度の設定
		meshAnim.animSpeed = ANIMATION_DEFAULT;

		// アニメーションの変更
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);

		return new PaidWaitState;
	}
	return nullptr;
}

void HealState::GuiUpdate(void)
{
	ImGui::Text("Heal");
}
