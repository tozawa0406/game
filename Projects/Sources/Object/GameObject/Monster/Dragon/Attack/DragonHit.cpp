#include "DragonHit.h"
#include "../Dragon.h"

DragonHit::DragonHit(void)
{
}

DragonHit::~DragonHit(void)
{
}

void DragonHit::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);
}

void DragonHit::Uninit(void)
{
}

void DragonHit::SetMove(void)
{
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	meshAnim.animSpeed = 0.5f;
	meshAnim.animation   = static_cast<int>(Dragon::Animation::HIT);

	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
}

bool DragonHit::Update(void)
{
	if (!monster_) { return true; }

	monster_->SetVelocity(VECTOR3(0));

	if (monster_->IsEndAnim())
	{
		auto& meshAnim = monster_->GetMeshAnimation();

		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 5, true);
		return true;
	}
	return false;
}

void DragonHit::EndMove(void)
{
}
