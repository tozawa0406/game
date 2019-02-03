#include "DragonWingAttack.h"
#include "../Dragon.h"

//! @def	�_���[�W�l
static constexpr int DAMAGE = 20;

//! @def	�A�j���[�V�������x
static constexpr float ANIMATION_SLOWLY			= 0.2f;
//! @def	�A�j���[�V�������x
static constexpr float ANIMATION_MORE_SLOWLY	= 0.02f;
//! @def	�A�j���[�V�������x
static constexpr float ANIMATION_HALF			= 0.5f;
//! @def	�A�j���[�V�������x
static constexpr float ANIMATION_BACK			= 0.3f;

//! @def	�A�j���[�V�������x�ύX�^�C�~���O
static constexpr int CHANGE_ANIMATION_QUICKLY	= 10;
//! @def	�A�j���[�V�������x�ύX�^�C�~���O
static constexpr int CHANGE_ANIMATION_HALF		= 20;
//! @def	�A�j���[�V�������x�ύX�^�C�~���O
static constexpr int CHANGE_ANIMATION_STOP		= 29;
//! @def	�A�j���[�V�������x�ύX�^�C�~���O
static constexpr int CHANGE_ANIMATION_SLOWLY	= 30;
//! @def	�A�j���[�V�������x�ύX�^�C�~���O
static constexpr int CHANGE_ANIMATION_BACK		= 33;

DragonWingAttack::DragonWingAttack(void)
{
	for (auto& c : collider_) { c = nullptr; }
}

DragonWingAttack::~DragonWingAttack(void)
{
}

void DragonWingAttack::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);

	Dragon* temp = static_cast<Dragon*>(monster);

	const int max = static_cast<int>(Wing::MAX);
	Dragon::Collision collisionNum[max] = 
	{
		Dragon::Collision::WING_L_CLAW, 
		Dragon::Collision::WING_LC, 
		Dragon::Collision::WING_LR,
		Dragon::Collision::WING_R_CLAW, 
		Dragon::Collision::WING_RC, 
		Dragon::Collision::WING_RR,
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

void DragonWingAttack::Uninit(void)
{
	for (auto& c : collider_) { DeletePtr(c); }
}

void DragonWingAttack::SetMove(void)
{
	// ���Ɏg�p���Ȃ�d���h�~
	if (enable_ || !monster_) { return; }

	auto& meshAnim = monster_->GetMeshAnimation();

	MonsterAttack::SetMove();

	// ���x�̐ݒ�
	meshAnim.animSpeed = ANIMATION_SLOWLY;

	// �A�j���[�V�����̐ݒ�
	meshAnim.animation = static_cast<int>(Dragon::Animation::WING_ATTACK);

	// ���ۂ̃A�j���[�V�����̐؂�ւ�
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

	const auto& attackManager = monster_->GetAttackManager();
	if (!attackManager) { return; }
	attackID_ = attackManager->CreateAttackID();
}

bool DragonWingAttack::Update(void)
{
	if (!monster_) { return true; }

	// �ړ������Ȃ�
	monster_->SetVelocity(VECTOR3(0));

	auto& meshAnim = monster_->GetMeshAnimation();
	float p = meshAnim.mesh.GetPattern();
	// ���̎��Ԃ𒴂�����A�j���[�V�������x��ς���
	if (p > CHANGE_ANIMATION_BACK)
	{
		meshAnim.animSpeed = ANIMATION_BACK;
	}
	else if (p > CHANGE_ANIMATION_SLOWLY)
	{
		meshAnim.animSpeed = ANIMATION_SLOWLY;
	}
	else if (p > CHANGE_ANIMATION_STOP)
	{
		meshAnim.animSpeed = ANIMATION_MORE_SLOWLY;
		for (auto& c : collider_) { c->SetEnable(false); }
	}
	else if (p > CHANGE_ANIMATION_HALF)
	{
		meshAnim.animSpeed = ANIMATION_HALF;
	}
	else if (p > CHANGE_ANIMATION_QUICKLY)
	{
		meshAnim.animSpeed = DEFAULT_ANIMATION_SPEED;
		for (auto& c : collider_) { c->SetEnable(true); }
	}

	for (auto& collider : collider_)
	{
		const auto& hits = collider->HitCollider();
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

	// �A�j���[�V�����I��
	if (monster_->IsEndAnim())
	{
		// ���ɖ߂�
		meshAnim.animSpeed = DEFAULT_ANIMATION_SPEED;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30, true);
		return true;
	}
	return false;
}

void DragonWingAttack::EndMove(void)
{
	for (auto& collider : collider_)
	{
		collider->SetEnable(false);
	}
}

void DragonWingAttack::GuiUpdate(void)
{
	MonsterAttack::GuiUpdate();

	const auto& meshAnim = monster_->GetMeshAnimation();
	ImGui::Text("pattern : %.2f", meshAnim.mesh.GetPattern());
}
