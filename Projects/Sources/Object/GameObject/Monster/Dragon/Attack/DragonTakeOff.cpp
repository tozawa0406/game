#include "DragonTakeOff.h"
#include "../Dragon.h"

DragonTakeOff::DragonTakeOff(void) : 
	fly_(false)
	, debug_velocity_(0)
{
}

DragonTakeOff::~DragonTakeOff(void)
{
}

void DragonTakeOff::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);
}

void DragonTakeOff::Uninit(void)
{
}

void DragonTakeOff::SetMove(void)
{
	// 既に使用中なら重複防止
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	// 速度の設定
	meshAnim.animSpeed = fly_ ? -0.75f : 0.75f;

	// アニメーションの設定
	meshAnim.animation = static_cast<int>(Dragon::Animation::TAKE_OFF);

	// 実際のアニメーションの切り替え
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
}

bool DragonTakeOff::Update(void)
{
	if (!monster_) { return true; }

	auto velocity = monster_->GetVelocity();
	int sign = (fly_) ? -1 : 1;
	velocity.x = 0;
	velocity.z = 0;
	velocity.y += 0.1f * sign;
	monster_->SetVelocity(velocity);

	debug_velocity_ = velocity.y;

	// アニメーション終了
	if (monster_->IsEndAnim())
	{
		auto& meshAnim = monster_->GetMeshAnimation();
		// 元に戻す
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>((fly_) ? Dragon::Animation::WAIT1 : Dragon::Animation::FLY_IDLE);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 5, true);
		return true;
	}
	return false;
}

void DragonTakeOff::EndMove(void)
{
}

void DragonTakeOff::GuiUpdate(void)
{
	ImGui::Text("velocity : %.2f", debug_velocity_);
	ImGui::Text("y : %.2f", monster_->GetTransform().position.y);
}
