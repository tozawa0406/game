#include "DragonScream.h"
#include "../Dragon.h"

DragonScream::DragonScream(void)
{
}

DragonScream::~DragonScream(void)
{
}

void DragonScream::Init(Object* object)
{
	UNREFERENCED_PARAMETER(object);
}

void DragonScream::Uninit(void)
{
}

void DragonScream::SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum)
{
	if (enable_) { return; }

	MonsterAttack::SetMove(mesh, animSpeed, animNum);

	animSpeed = 0.75f;
	animNum   = static_cast<int>(Dragon::Animation::SCREAM);

	mesh.ChangeAnimation(animNum, 15);
}

bool DragonScream::Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd)
{
	UNREFERENCED_PARAMETER(trans);

	velocity = VECTOR3(0);

	if (animEnd)
	{
		animSpeed = 0.75f;
		animNum = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		mesh.ChangeAnimation(animNum, 5, true);
		return true;
	}
	return false;
}
