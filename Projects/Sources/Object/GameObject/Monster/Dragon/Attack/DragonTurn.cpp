#include "DragonTurn.h"
#include "../Dragon.h"

//! @def	ダメージ値
static constexpr int DAMAGE = 20;

//! @def	アニメーション速度変更タイミング
static constexpr int CHANGE_ANIMATION_QUICKLY	= 5;
//! @def	アニメーション速度変更タイミング
static constexpr int CHANGE_ANIMATION_STOP		= 15;
//! @def	アニメーション速度変更タイミング
static constexpr int CHANGE_ANIMATION_SLOWLY	= 16;
//! @def	アニメーション速度変更タイミング
static constexpr int CHANGE_ANIMATION_BACK		= 22;

DragonTurn::DragonTurn(void)
{
	for (auto& c : collider_) { c = nullptr; }
}

DragonTurn::~DragonTurn(void)
{
}

void DragonTurn::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);

	Dragon* temp = static_cast<Dragon*>(monster);

	const int max = static_cast<int>(Collision::MAX);
	Dragon::Collision collisionNum[max] =
	{
		Dragon::Collision::WING_LR,
		Dragon::Collision::WING_LC,
		Dragon::Collision::WING_RR,
		Dragon::Collision::WING_RC,
		Dragon::Collision::TAIL5,
		Dragon::Collision::TAIL4,
		Dragon::Collision::TAIL3,
		Dragon::Collision::TAIL2,
		Dragon::Collision::TAIL1,
	};

	for (int i = 0; i < max; ++i)
	{
		const auto& tempCol = temp->GetCollider(collisionNum[i]);
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

void DragonTurn::Uninit(void)
{
	for (auto& c : collider_) { DeletePtr(c); }
}

void DragonTurn::SetMove(void)
{
	// 既に使用中なら重複防止
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	// 速度の設定
	meshAnim.animSpeed	 = 0.2f;

	// アニメーションの設定
	meshAnim.animation   = static_cast<int>(Dragon::Animation::TAIL_ATTACK);

	// 実際の切り替え
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
}

bool DragonTurn::Update(void)
{
	if (!monster_) { return true; }

	// 移動はさせない
	monster_->SetVelocity(VECTOR3(0));

	auto& meshAnim = monster_->GetMeshAnimation();

	// 一定の時間を超えたらアニメーション速度を変える
	float p = meshAnim.mesh.GetPattern();
	if (p > 45)
	{
		meshAnim.animSpeed = 0.2f;
	}
	else if (p > 44)
	{
		for (auto& c : collider_) { if (c) { c->SetEnable(false); } }
		meshAnim.animSpeed = 0.02f;
	}
	else if (p > 40)
	{
		meshAnim.animSpeed = 0.5f;
	}
	else if (p > 10)
	{
		for (auto& c : collider_) { if (c) { c->SetEnable(true); } }
		meshAnim.animSpeed = DEFAULT_ANIMATION_SPEED;
	}

	for (auto& c : collider_)
	{
		const auto& hits = c->HitCollider();
		for (auto& hit : hits)
		{
			if (hit->GetParentTag() == ObjectTag::PLAYER &&
				hit->GetColliderTag() == ColliderTag::DEFENSE)
			{
				if (attackManager_->CheckList(attackID_))
				{
					static_cast<GameObject*>(hit->GetParent())->Hit(DAMAGE, attackID_);
				}
			}
		}
	}

	// アニメーション終了
	if (monster_->IsEndAnim())
	{
		// 元に戻す
		meshAnim.animSpeed = DEFAULT_ANIMATION_SPEED;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME5, true);

		Dragon* temp = static_cast<Dragon*>(monster_);
		if (!temp) { return true; }
		Transform trans = temp->GetTransform();
		trans.rotation.y += (trans.rotation.y > 3.14f) ? -3.14f : 3.14f;
		trans.position += temp->GetFront() * 12.25f;
		temp->SetPosition(trans.position);
		temp->SetRotation(trans.rotation);

		return true;
	}
	return false;
}

void DragonTurn::EndMove(void)
{
}

void DragonTurn::GuiUpdate(void)
{
	MonsterAttack::GuiUpdate();

	auto& meshAnim = monster_->GetMeshAnimation();
	ImGui::Text("pattern : %.2f", meshAnim.mesh.GetPattern());
	ImGui::Text("speed : %.2f", meshAnim.animSpeed);
}
