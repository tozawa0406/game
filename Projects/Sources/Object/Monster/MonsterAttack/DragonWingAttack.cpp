#include "DragonWingAttack.h"
#include "../Dragon.h"

DragonWingAttack::DragonWingAttack(void)
{
}

DragonWingAttack::~DragonWingAttack(void)
{
}

void DragonWingAttack::Init(Object* object)
{
	UNREFERENCED_PARAMETER(object);
}

void DragonWingAttack::Uninit(void)
{
}

void DragonWingAttack::SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum)
{
	if (enable_) { return; }

	MonsterAttack::SetMove(mesh, animSpeed, animNum);

	animSpeed = 0.75f;
	animNum   = static_cast<int>(Dragon::Animation::WING_ATTACK);

	mesh.ChangeAnimation(animNum, 15);
}

bool DragonWingAttack::Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd)
{
	UNREFERENCED_PARAMETER(trans);

	velocity = VECTOR3(0);

	if (animEnd)
	{
		animSpeed = 0.75f;
		animNum = static_cast<int>(Dragon::Animation::WAIT);
		enable_ = false;
		mesh.ChangeAnimation(animNum, 5, true);
		return true;
	}
	return false;
}
