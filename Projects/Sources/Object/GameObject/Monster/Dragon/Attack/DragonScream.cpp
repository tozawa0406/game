#include "DragonScream.h"
#include <random>
#include "../Dragon.h"
#include "../../../Player/Player.h"

DragonScream::DragonScream(void) : 
	collider_(nullptr)
{
}

DragonScream::~DragonScream(void)
{
}

void DragonScream::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);

	collider_ = new Collider3D::Sphere(monster);
	if (collider_)
	{
		collider_->SetSize(125);
		collider_->SetEnable(false);
	}
}

void DragonScream::Uninit(void)
{
	DeletePtr(collider_);
}

void DragonScream::SetMove(void)
{
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	meshAnim.animSpeed = 0.75f;
	meshAnim.animation   = static_cast<int>(Dragon::Animation::SCREAM);

	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);

	for (;;)
	{
		std::random_device randDev;
		uint8 attack = static_cast<uint8>(randDev() % 255);

		if (attack_ == attack) { continue; }

		attack_ = attack;
		break;
	}
}

bool DragonScream::Update(void)
{
	if (!monster_) { return true; }

	monster_->SetVelocity(VECTOR3(0));

	if (collider_)
	{
		const auto& objects = collider_->Hit();
		for (auto& object : objects)
		{
			if (object && object->GetTag() == ObjectTag::PLAYER)
			{
				Player* player = static_cast<Player*>(object);
				player->Earplug(attack_);
			}
		}
	}

	RenderTarget* rtv = nullptr;
	if (const auto& sys = Systems::Instance())
	{
		if (const auto& gra = sys->GetGraphics())
		{
			rtv = gra->GetRenderTarget();
		}
	}

	if (!rtv) { return false; }

	auto& meshAnim = monster_->GetMeshAnimation();
	float pattern = meshAnim.mesh.GetPattern();
	if (pattern > 80)
	{
		rtv->FeedbackBlur(false);
	}
	else if (pattern > 20)
	{
		rtv->FeedbackBlur(true);
		if (collider_)
		{
			collider_->SetEnable(true);
		}

		meshAnim.animSpeed = 0.5f;
	}

	if (monster_->IsEndAnim())
	{
		rtv->FeedbackBlur(false);

		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 5, true);
		return true;
	}
	return false;
}

void DragonScream::EndMove(void)
{
	if (collider_)
	{
		collider_->SetEnable(false);
	}
}

void DragonScream::GuiUpdate(void)
{
	if (!monster_) { return; }
	auto& meshAnim = monster_->GetMeshAnimation();
	ImGui::Text("pattern : %.2f", meshAnim.mesh.GetPattern());

	MonsterAttack::GuiUpdate();
}

