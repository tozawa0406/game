#include "DragonRush.h"
#include "../Dragon.h"

DragonRush::DragonRush(void) :
	cnt_(0)
{
}

DragonRush::~DragonRush(void)
{
}

void DragonRush::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);
}

void DragonRush::Uninit(void)
{
}

void DragonRush::SetMove(void)
{
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	meshAnim.animSpeed = 0.5f;
	meshAnim.animation   = static_cast<int>(Dragon::Animation::WAIT2);

	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
	cnt_ = 0;
}

bool DragonRush::Update(void)
{
	if (!monster_) { return true; }
	auto& meshAnim = monster_->GetMeshAnimation();

	cnt_++;
	if (cnt_ > 30)
	{
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::RUN);

		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);

		VECTOR3 front = monster_->GetFront();
		monster_->SetVelocity(-front);
	}

	if (cnt_ > 148)
	{
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15, true);
		return true;
	}
	return false;
}

void DragonRush::EndMove(void)
{
}

void DragonRush::GuiUpdate(void)
{
	MonsterAttack::GuiUpdate();

	ImGui::Text("cnt : %d", cnt_);
}
