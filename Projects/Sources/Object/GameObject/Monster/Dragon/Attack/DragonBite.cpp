#include "DragonBite.h"
#include "../Dragon.h"
#include <FrameWork/Graphics/DirectX11/Dx11Wrapper.h>

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

//! @def	���̃{�[���̖��O
static const     string BONE_HEAD = "Head";
static const     VECTOR3 COLLISION_OFFSET_POS_HEAD = VECTOR3(5, 3, 0);
static const     VECTOR3 COLLISION_SIZE_HEAD = VECTOR3(6.1f, 6.1f, 6.1f);

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

	// �����蔻��̒ǉ�
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& renderer = systems->GetGraphics())
		{
			if (Dx11Wrapper* wrapper = static_cast<Dx11Wrapper*>(renderer->GetWrapper()))
			{
				const auto& model = wrapper->GetModel(static_cast<int>(Resources::Model::Buttle::DRAGON));

				collider_ = new Collider3D::OBB(monster);
				if (collider_)
				{
					for (auto& bone : model.bone)
					{
						if (bone.name == BONE_HEAD)
						{
							collider_->SetParentMtx(&model.transMtx, &bone.nowBone);
							break;
						}
					}
					const auto& s = monster->GetTransform().scale;
					collider_->SetOffsetPosition(COLLISION_OFFSET_POS_HEAD * s);
					collider_->SetSize(COLLISION_SIZE_HEAD * s);
					collider_->SetColliderTag(ColliderTag::ATTACK);
					collider_->SetTrigger(true);
					collider_->SetEnable(false);
				}
			}
		}
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
			static_cast<GameObject*>(hit->GetParent())->Hit(DAMAGE);
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
