#include "DragonBite.h"
#include "../Dragon.h"

//! @def	ダメージ値
static constexpr int DAMAGE = 30;

//! @def	アニメーション速度
static constexpr float ANIMATION_SLOWLY			= 0.2f;
//! @def	アニメーション速度
static constexpr float ANIMATION_MORE_SLOWLY	= 0.02f;
//! @def	アニメーション速度
static constexpr float ANIMATION_HALF			= 0.5f;

//! @def	アニメーション速度変更タイミング
static constexpr int CHANGE_ANIMATION_QUICKLY	= 5;
//! @def	アニメーション速度変更タイミング
static constexpr int CHANGE_ANIMATION_STOP		= 15;
//! @def	アニメーション速度変更タイミング
static constexpr int CHANGE_ANIMATION_SLOWLY	= 16;
//! @def	アニメーション速度変更タイミング
static constexpr int CHANGE_ANIMATION_BACK		= 22;

DragonBite::DragonBite(void) : 
	collider_(nullptr)
{
}

DragonBite::~DragonBite(void)
{
}

void DragonBite::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);

	Dragon* temp = static_cast<Dragon*>(monster);

	const auto& tempCol = temp->GetCollider(Dragon::Collision::HEAD);
	auto& col = collider_;
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

void DragonBite::Uninit(void)
{
	DeletePtr(collider_);
}

void DragonBite::SetMove(void)
{
	// 既に使用中なら重複防止
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	// 速度の設定
	meshAnim.animSpeed	 = ANIMATION_SLOWLY;

	// アニメーションの設定
	meshAnim.animation   = static_cast<int>(Dragon::Animation::BITE);

	// 実際の切り替え
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
}

bool DragonBite::Update(void)
{
	if (!monster_) { return true; }

	// 移動はさせない
	monster_->SetVelocity(VECTOR3(0));

	collider_->Update();

	auto& meshAnim = monster_->GetMeshAnimation();

	// 一定の時間を超えたらアニメーション速度を変える
	float p = meshAnim.mesh.GetPattern();
	// 引く速度よりは早く元の位置に戻す
	if (p > CHANGE_ANIMATION_BACK)
	{
		meshAnim.animSpeed = ANIMATION_HALF;
	}
	// 首をゆっくり引く
	else if (p > CHANGE_ANIMATION_SLOWLY)
	{
		meshAnim.animSpeed = ANIMATION_SLOWLY;
	}
	// 攻撃後軽く止まる
	else if (p > CHANGE_ANIMATION_STOP)
	{
		meshAnim.animSpeed = ANIMATION_MORE_SLOWLY;
		collider_->SetEnable(false);
	}
	// 実際の攻撃のため早く
	else if (p > CHANGE_ANIMATION_QUICKLY)
	{
		meshAnim.animSpeed = DEFAULT_ANIMATION_SPEED;
		collider_->SetEnable(true);
	}

	const auto& hits = collider_->HitCollider();
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

	// アニメーション終了
	if (monster_->IsEndAnim())
	{
		// 元に戻す
		meshAnim.animSpeed = DEFAULT_ANIMATION_SPEED;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME5, true);
		return true;
	}
	return false;
}

void DragonBite::EndMove(void)
{
	collider_->SetEnable(false);
}

void DragonBite::GuiUpdate(void)
{
	MonsterAttack::GuiUpdate();

	auto& meshAnim = monster_->GetMeshAnimation();
	ImGui::Text("pattern : %.2f", meshAnim.mesh.GetPattern());
	ImGui::Text("speed : %.2f", meshAnim.animSpeed);
}
