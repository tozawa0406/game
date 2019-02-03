#include "DragonBite.h"
#include "../Dragon.h"

//! @def	�_���[�W�l
static constexpr int DAMAGE = 30;

//! @def	�A�j���[�V�������x
static constexpr float ANIMATION_SLOWLY			= 0.2f;
//! @def	�A�j���[�V�������x
static constexpr float ANIMATION_MORE_SLOWLY	= 0.02f;
//! @def	�A�j���[�V�������x
static constexpr float ANIMATION_HALF			= 0.5f;

//! @def	�A�j���[�V�������x�ύX�^�C�~���O
static constexpr int CHANGE_ANIMATION_QUICKLY	= 5;
//! @def	�A�j���[�V�������x�ύX�^�C�~���O
static constexpr int CHANGE_ANIMATION_STOP		= 15;
//! @def	�A�j���[�V�������x�ύX�^�C�~���O
static constexpr int CHANGE_ANIMATION_SLOWLY	= 16;
//! @def	�A�j���[�V�������x�ύX�^�C�~���O
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
	// ���Ɏg�p���Ȃ�d���h�~
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	// ���x�̐ݒ�
	meshAnim.animSpeed	 = ANIMATION_SLOWLY;

	// �A�j���[�V�����̐ݒ�
	meshAnim.animation   = static_cast<int>(Dragon::Animation::BITE);

	// ���ۂ̐؂�ւ�
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
}

bool DragonBite::Update(void)
{
	if (!monster_) { return true; }

	// �ړ��͂����Ȃ�
	monster_->SetVelocity(VECTOR3(0));

	collider_->Update();

	auto& meshAnim = monster_->GetMeshAnimation();

	// ���̎��Ԃ𒴂�����A�j���[�V�������x��ς���
	float p = meshAnim.mesh.GetPattern();
	// �������x���͑������̈ʒu�ɖ߂�
	if (p > CHANGE_ANIMATION_BACK)
	{
		meshAnim.animSpeed = ANIMATION_HALF;
	}
	// �������������
	else if (p > CHANGE_ANIMATION_SLOWLY)
	{
		meshAnim.animSpeed = ANIMATION_SLOWLY;
	}
	// �U����y���~�܂�
	else if (p > CHANGE_ANIMATION_STOP)
	{
		meshAnim.animSpeed = ANIMATION_MORE_SLOWLY;
		collider_->SetEnable(false);
	}
	// ���ۂ̍U���̂��ߑ���
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

	// �A�j���[�V�����I��
	if (monster_->IsEndAnim())
	{
		// ���ɖ߂�
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
