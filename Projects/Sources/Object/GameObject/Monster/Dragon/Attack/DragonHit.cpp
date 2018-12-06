#include "DragonHit.h"
#include "../Dragon.h"

/* @brief	�R���X�g���N�^			*/
DragonHit::DragonHit(void)
{
}

/* @brief	�f�X�g���N�^			*/
DragonHit::~DragonHit(void)
{
}

/* @brief	����������
 * @param	(object)	�����蔻��̐e�N���X�ɓo�^
 * @return	�Ȃ�					*/
void DragonHit::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);
}

/* @brief	�㏈��					*/
void DragonHit::Uninit(void)
{
}

/* @brief	���s���鏀��
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���̍U�����L�̃A�j���[�V�����A���x��ݒ肷��		*/
void DragonHit::SetMove(void)
{
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	meshAnim.animSpeed = 0.5f;
	meshAnim.animation   = static_cast<int>(Dragon::Animation::HIT);

	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�U�����I��������true
 * @detail	�p���ύX�͂Ȃ��A�ړ����Ȃ��悤���x��0�ɌŒ�
			��莞�Ԍo�ߌ�A�A�j���[�V�����̑��x���グ��A�A�j���[�V�������I�������猳�ɖ߂��ďI��		*/
bool DragonHit::Update(void)
{
	if (!monster_) { return true; }

	monster_->SetVelocity(VECTOR3(0));

	if (monster_->IsEndAnim())
	{
		auto& meshAnim = monster_->GetMeshAnimation();

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
 * @return	�Ȃ�					*/
void DragonHit::EndMove(void)
{
}
