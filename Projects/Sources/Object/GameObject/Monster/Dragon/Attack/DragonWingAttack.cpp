#include "DragonWingAttack.h"
#include "../Dragon.h"
#include <FrameWork/Graphics/DirectX11/Dx11Wrapper.h>

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

//! @def	���ܗ��̃{�[���̖��O
static const     string  BONE_CLAW_L = "WingClaw2_L";
static const     VECTOR3 COLLISION_OFFSET_POS_CLAW_L = VECTOR3(0, -3, -4);
static const     VECTOR3 COLLISION_OFFSET_ROT_CLAW_L = VECTOR3(0, -2, 0);
static const     VECTOR3 COLLISION_SIZE_CLAW_L = VECTOR3(7.1f, 2.1f, 2.1f);
//! @def	�E�ܗ��̃{�[���̖��O
static const     string  BONE_CLAW_R = "WingClaw2_R";
static const     VECTOR3 COLLISION_OFFSET_POS_CLAW_R = VECTOR3(0, 3, 4);
static const     VECTOR3 COLLISION_OFFSET_ROT_CLAW_R = VECTOR3(0, -2, 0);
static const     VECTOR3 COLLISION_SIZE_CLAW_R = VECTOR3(7.1f, 2.1f, 2.1f);

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

	// �����蔻��̐���
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& renderer = systems->GetGraphics())
		{
			if (Dx11Wrapper* wrapper = static_cast<Dx11Wrapper*>(renderer->GetWrapper()))
			{
				const auto& model = wrapper->GetModel(static_cast<int>(Resources::Model::Buttle::DRAGON));

				int num = static_cast<int>(Wing::CLAW_L);
				collider_[num] = new Collider3D::OBB(monster);
				if (collider_[num])
				{
					for (auto& bone : model.bone)
					{
						if (bone.name == BONE_CLAW_L)
						{
							collider_[num]->SetParentMtx(&model.transMtx, &bone.nowBone);
							break;
						}
					}
					const auto& s = monster->GetTransform().scale;
					collider_[num]->SetOffsetPosition(COLLISION_OFFSET_POS_CLAW_L * s);
					collider_[num]->SetOffsetRotation(COLLISION_OFFSET_ROT_CLAW_L);
					collider_[num]->SetSize(COLLISION_SIZE_CLAW_L * s);
					collider_[num]->SetColliderTag(ColliderTag::ATTACK);
					collider_[num]->SetTrigger(true);
					collider_[num]->SetEnable(false);
				}

				num = static_cast<int>(Wing::CLAW_R);
				collider_[num] = new Collider3D::OBB(monster);
				if (collider_[num])
				{
					for (auto& bone : model.bone)
					{
						if (bone.name == BONE_CLAW_R)
						{
							collider_[num]->SetParentMtx(&model.transMtx, &bone.nowBone);
							break;
						}
					}
					const auto& s = monster->GetTransform().scale;
					collider_[num]->SetOffsetPosition(COLLISION_OFFSET_POS_CLAW_R * s);
					collider_[num]->SetOffsetRotation(COLLISION_OFFSET_ROT_CLAW_R);
					collider_[num]->SetSize(COLLISION_SIZE_CLAW_R * s);
					collider_[num]->SetColliderTag(ColliderTag::ATTACK);
					collider_[num]->SetTrigger(true);
					collider_[num]->SetEnable(false);
				}
			}
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
		for (auto& c : collider_) { c->SetEnable(false); }
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
				static_cast<GameObject*>(hit->GetParent())->Hit(DAMAGE);
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
