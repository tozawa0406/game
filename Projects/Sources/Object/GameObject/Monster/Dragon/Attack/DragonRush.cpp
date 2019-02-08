#include "DragonRush.h"
#include "../Dragon.h"

//! @def	ダメージ値
static constexpr int DAMAGE = 40;

//! @def	アニメーション速度
static constexpr float ANIMATION_SLOWLY = 0.2f;
//! @def	アニメーション速度
static constexpr float ANIMATION_MORE_SLOWLY = 0.02f;

//! @def	ストップ後の速度係数
static constexpr float MOVE_SPEED_STOP = 0.75f;
//! @def	ストップ後1フレームに減速する量
static constexpr float MOVE_SPEED_STOP_ONE = 0.025f;

//! @def	走り始めるタイミング
static constexpr int START_RUN = 60;
//! @def	走り終わるタイミング
static constexpr int STOP_RUN = 170;
//! @def	攻撃終わりのタイミング
static constexpr int END_RUN = 200;

DragonRush::DragonRush(void) :
	cnt_(0)
{
	for (auto& c : collider_) { c = nullptr; }
}

DragonRush::~DragonRush(void)
{
}

void DragonRush::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);

	Dragon* temp = static_cast<Dragon*>(monster);
	if (!temp) { return; }
	int max = static_cast<int>(Collision::MAX);
	for (int i = 0; i < max; ++i)
	{
		const auto& tempCol = temp->GetCollider(static_cast<Dragon::Collision>(i));
		auto& col = collider_[i];
		col = new Collider3D::OBB(monster);
		if (col)
		{
			col->SetSize(tempCol->GetSize());
			col->SetOffsetPosition(tempCol->GetOffsetPosition() + VECTOR3(0.1f));
			col->SetOffsetRotation(tempCol->GetOffsetRotation());
			col->SetParentMtx(tempCol->GetTransMtx(), tempCol->GetParentMtx());
			col->SetColliderTag(ColliderTag::ATTACK);
			col->SetTrigger(true);
			col->SetEnable(false);
		}
	}
}

void DragonRush::Uninit(void)
{
	for (auto& c : collider_)
	{
		DeletePtr(c);
	}
}

void DragonRush::SetMove(void)
{
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	meshAnim.animSpeed = ANIMATION_SLOWLY;
	meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT2);

	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
	cnt_ = 0;
}

bool DragonRush::Update(void)
{
	if (!monster_) { return true; }
	auto& meshAnim = monster_->GetMeshAnimation();

	monster_->SetVelocity(VECTOR3(0));

	cnt_++;
	if (cnt_ > END_RUN)
	{
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30, true);
		return true;
	}
	else if (cnt_ > STOP_RUN)
	{
		for (auto& c : collider_) { if (c) { c->SetEnable(false); } }
		meshAnim.animSpeed = ANIMATION_MORE_SLOWLY;
		VECTOR3 front = monster_->GetFront();
		float adjust = MOVE_SPEED_STOP - (MOVE_SPEED_STOP_ONE * (cnt_ - STOP_RUN));
		monster_->SetVelocity(-front * adjust);
	}
	else if (cnt_ > START_RUN)
	{
		for (auto& c : collider_) { if (c) { c->SetEnable(true); } }
		meshAnim.animSpeed = DEFAULT_ANIMATION_SPEED;
		meshAnim.animation = static_cast<int>(Dragon::Animation::RUN);

		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

		VECTOR3 front = monster_->GetFront();
		monster_->SetVelocity(-front);
	}

	for (auto& c : collider_) 
	{
		if (c)
		{
			c->Update(); 
			const auto& hits = c->HitCollider();
			for (auto& hit : hits)
			{
				if (hit->GetParentTag() == ObjectTag::PLAYER &&
					hit->GetColliderTag() == ColliderTag::DEFENSE)
				{
					if (attackManager_->CheckList(attackID_))
					{
						static_cast<GameObject*>(hit->GetParent())->Hit(DAMAGE, attackID_);

						Dragon* dragon = static_cast<Dragon*>(monster_);
						if (!dragon) { return false; }

						dragon->AddIgnore(ObjectTag::PLAYER);
						break;
					}
				}
			}

		}
	}

	return false;
}

void DragonRush::EndMove(void)
{
	Dragon* dragon = static_cast<Dragon*>(monster_);
	if (!dragon) { return; }

	dragon->RemoveIgnore(ObjectTag::PLAYER);
}

void DragonRush::GuiUpdate(void)
{
	MonsterAttack::GuiUpdate();

	ImGui::Text("cnt : %d", cnt_);
}
