#include "DragonScream.h"
#include "../Dragon.h"

DragonScream::DragonScream(void)
{
}

DragonScream::~DragonScream(void)
{
}

void DragonScream::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);
}

void DragonScream::Uninit(void)
{
}

void DragonScream::SetMove(void)
{
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	meshAnim.animSpeed = 0.75f;
	meshAnim.animation   = static_cast<int>(Dragon::Animation::SCREAM);

	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
}

bool DragonScream::Update(void)
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
