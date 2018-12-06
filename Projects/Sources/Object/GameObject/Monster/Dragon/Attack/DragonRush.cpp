#include "DragonRush.h"
#include "../Dragon.h"

/* @brief	�R���X�g���N�^			*/
DragonRush::DragonRush(void) : cnt_(0)
{
}

/* @brief	�f�X�g���N�^			*/
DragonRush::~DragonRush(void)
{
}

/* @brief	����������
 * @param	(object)	�����蔻��̐e�N���X�ɓo�^
 * @return	�Ȃ�					*/
void DragonRush::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void DragonRush::Uninit(void)
{
}

/* @brief	���s���鏀��
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���̍U�����L�̃A�j���[�V�����A���x��ݒ肷��		*/
void DragonRush::SetMove(void)
{
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	meshAnim.animSpeed = 0.5f;
	meshAnim.animation   = static_cast<int>(Dragon::Animation::WAIT2);

	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
	cnt_ = 0;
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	�U�����I��������true
 * @detail	�p���ύX�͂Ȃ��A�ړ����Ȃ��悤���x��0�ɌŒ�
			��莞�Ԍo�ߌ�A�A�j���[�V�����̑��x���グ��A�A�j���[�V�������I�������猳�ɖ߂��ďI��		*/
bool DragonRush::Update(void)
{
	if (!monster_) { return true; }
	auto& meshAnim = monster_->GetMeshAnimation();

	cnt_++;
	if (cnt_ > 30)
	{
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::RUN);

		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);

		VECTOR3 front = monster_->GetFront();
		monster_->SetVelocity(-front);
	}

	if (cnt_ > 148)
	{
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15, true);
		return true;
	}
	return false;
}

/* @brief	���[�V�����̏I������
 * @param	�Ȃ�
 * @return	�Ȃ�			*/
void DragonRush::EndMove(void)
{
}

/* @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�U�����I�u�W�F�N�g����Ă΂��		*/
void DragonRush::GuiUpdate(void)
{
	MonsterAttack::GuiUpdate();

	ImGui::Text("cnt : %d", cnt_);
}
