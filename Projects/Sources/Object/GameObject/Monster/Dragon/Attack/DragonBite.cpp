#include "DragonBite.h"
#include "../Dragon.h"
#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>

//! @def	�A�j���[�V�����̑��x��ς���(��������)�^�C�~���O
static constexpr int CHANGE_FRAME = 20;
//! @def	���݂��̏I��(�����蔻��̏���)
static constexpr int END_ATTACK   = 40;

//! @def	���̃{�[���̖��O
static const     string BONE_HEAD = "Head";
static const     VECTOR3 COLLISION_OFFSET_POS_HEAD = VECTOR3(5, 3, 0);
static const     VECTOR3 COLLISION_SIZE_HEAD = VECTOR3(6.1f, 6.1f, 6.1f);

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����		*/
DragonBite::DragonBite(void) : collider_(nullptr), debug_speed_(0), debug_changeFrame_(CHANGE_FRAME)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...					*/
DragonBite::~DragonBite(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	(object)	�����蔻��̐e�N���X�ɓo�^
 * @return	�Ȃ�				*/
void DragonBite::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);

	// �����蔻��̒ǉ�
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& renderer = systems->GetRenderer())
		{
			if (DirectX11Wrapper* wrapper = static_cast<DirectX11Wrapper*>(renderer->GetWrapper()))
			{
				const auto& model = wrapper->GetModel(static_cast<int>(Model::Game::DRAGON));

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
					collider_->SetRendererColor(COLOR(1, 0, 0, 1));
					collider_->SetEnable(false);
				}
			}
		}
	}
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void DragonBite::Uninit(void)
{
	DeletePtr(collider_);
}

/* @fn		SetMove
 * @brief	���s���鏀��
 * @param	(mesh)			�A�j���[�V������ύX���������b�V��
 * @param	(animSpeed)		�A�j���[�V�������x
 * @param	(animNum)		�A�j���[�V�����̔ԍ�
 * @return	�Ȃ�
 * @detail	���̍U�����L�̃A�j���[�V�����A���x��ݒ肷��		*/
void DragonBite::SetMove(void)
{
	// ���Ɏg�p���Ȃ�d���h�~
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	// ���x�̐ݒ�
	meshAnim.animSpeed	 = 0.4f;
	debug_speed_ = meshAnim.animSpeed;

	// �A�j���[�V�����̐ݒ�
	meshAnim.animation   = static_cast<int>(Dragon::Animation::BITE);

	// ���ۂ̐؂�ւ�
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�U�����I��������true
 * @detail	�p���ύX�͂Ȃ��A�ړ����Ȃ��悤���x��0�ɌŒ�
			��莞�Ԍo�ߌ�A�A�j���[�V�����̑��x���グ��A�A�j���[�V�������I�������猳�ɖ߂��ďI��		*/
bool DragonBite::Update(void)
{
	if (!monster_) { return true; }

	// �ړ��͂����Ȃ�
	monster_->SetVelocity(VECTOR3(0));

	collider_->Update();

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
		meshAnim.animSpeed	 = 0.75f;
		debug_speed_ = meshAnim.animSpeed;

		collider_->SetEnable(true);
	}

	if (frame_ > END_ATTACK)
	{
		collider_->SetEnable(false);
	}

	const auto& hits = collider_->Hit();
	for (auto& hit : hits)
	{
		if (hit->GetTag() == Object::Tag::PLAYER)
		{
			static_cast<GameObject*>(hit)->Hit(10);
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

/* @fn		GuiUpdate
 * @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�U�����I�u�W�F�N�g����Ă΂��		*/
void DragonBite::GuiUpdate(void)
{
	MonsterAttack::GuiUpdate();

	ImGui::Text("frame : %d", frame_);
	ImGui::Text("speed : %.2f", debug_speed_);
	ImGui::DragInt("changeFrame", &debug_changeFrame_);
}
