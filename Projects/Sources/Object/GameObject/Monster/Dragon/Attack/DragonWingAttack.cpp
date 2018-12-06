#include "DragonWingAttack.h"
#include "../Dragon.h"
#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>

//! @def	�A�j���[�V�����̑��x��ς���(��������)�^�C�~���O
static constexpr int CHANGE_FRAME = 18;
//! @def	�U���̏I��
static constexpr int END_ATTACK = 45;

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

/* @brief	�R���X�g���N�^			*/
DragonWingAttack::DragonWingAttack(void) : debug_speed_(0)
	, debug_changeFrame_(CHANGE_FRAME)
{
	for (auto& c : collider_) { c = nullptr; }
}

/* @brief	�f�X�g���N�^			*/
DragonWingAttack::~DragonWingAttack(void)
{
}

/* @brief	����������
 * @param	(object)	�����蔻��̐e�N���X�ɓo�^
 * @return	�Ȃ�					*/
void DragonWingAttack::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);

	// �����蔻��̐���
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& renderer = systems->GetRenderer())
		{
			if (DirectX11Wrapper* wrapper = static_cast<DirectX11Wrapper*>(renderer->GetWrapper()))
			{
				const auto& model = wrapper->GetModel(static_cast<int>(Model::Game::DRAGON));

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
					collider_[num]->SetRendererColor(COLOR(1, 0, 0, 1));
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
					collider_[num]->SetRendererColor(COLOR(1, 0, 0, 1));
					collider_[num]->SetEnable(false);
				}
			}
		}
	}

}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void DragonWingAttack::Uninit(void)
{
	for (auto& c : collider_) { DeletePtr(c); }
}

/* @brief	���s���鏀��
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���̍U�����L�̃A�j���[�V�����A���x��ݒ肷��		*/
void DragonWingAttack::SetMove(void)
{
	// ���Ɏg�p���Ȃ�d���h�~
	if (enable_ || !monster_) { return; }

	auto& meshAnim = monster_->GetMeshAnimation();

	MonsterAttack::SetMove();

	// ���x�̐ݒ�
	meshAnim.animSpeed	 = 0.6f;
	debug_speed_ = meshAnim.animSpeed;

	// �A�j���[�V�����̐ݒ�
	meshAnim.animation = static_cast<int>(Dragon::Animation::WING_ATTACK);

	// ���ۂ̃A�j���[�V�����̐؂�ւ�
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	�U�����I��������true
 * @detail	�p���ύX�͂Ȃ��A�ړ����Ȃ��悤���x��0�ɌŒ�
			��莞�Ԍo�ߌ�A�A�j���[�V�����̑��x���グ��A�A�j���[�V�������I�������猳�ɖ߂��ďI��		*/
bool DragonWingAttack::Update(void)
{
	if (!monster_) { return true; }

	// �ړ������Ȃ�
	monster_->SetVelocity(VECTOR3(0));

	// ���o�p
	frame_++;

	auto& meshAnim = monster_->GetMeshAnimation();

	// ���̎��Ԃ𒴂�����A�j���[�V�������x��ς���
#ifdef _SELF_DEBUG
	// �f�o�b�O�p
	if (frame_ > debug_changeFrame_)
#else
	if (frame_ > CHANGE_FRAME)
#endif
	{
		meshAnim.animSpeed    = 0.75f;
		debug_speed_ = meshAnim.animSpeed;

		for (auto& c : collider_) { c->SetEnable(true); }
	}

	if (frame_ > END_ATTACK)
	{
		for (auto& c : collider_) { c->SetEnable(false); }
	}

	for (auto& collider : collider_)
	{
		const auto& hits = collider->Hit();
		for (auto& hit : hits)
		{
			if (hit->GetTag() == Object::Tag::PLAYER)
			{
				static_cast<GameObject*>(hit)->Hit(40);
			}
		}
	}

	// �A�j���[�V�����I��
	if (monster_->IsEndAnim())
	{
		// ���ɖ߂�
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 5, true);
		return true;
	}
	return false;
}

/* @brief	���[�V�����̏I������
 * @param	�Ȃ�
 * @return	�Ȃ�			*/
void DragonWingAttack::EndMove(void)
{
	for (auto& collider : collider_)
	{
		collider->SetEnable(false);
	}
}

/* @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�U�����I�u�W�F�N�g����Ă΂��		*/
void DragonWingAttack::GuiUpdate(void)
{
	MonsterAttack::GuiUpdate();

	ImGui::Text("frame : %d", frame_);
	ImGui::Text("speed : %.2f", debug_speed_);
	ImGui::DragInt("changeFrame", &debug_changeFrame_);
}
