/*
 * @file		DragonBite.h
 * @brief		���̊��݂��U��
 * @author		���V�đ�
 * @data		2018/11/07
 */
#ifndef _DRAGON_BITE_H_
#define _DRAGON_BITE_H_

#include "../../MonsterAttack.h"

class DragonBite : public MonsterAttack
{
public:
	/* @brief	�R���X�g���N�^		*/
	DragonBite(void);
	/* @brief	�f�X�g���N�^		*/
	~DragonBite(void);


	
	/* @brief	����������
	 * @param	(monster)	�����蔻��̐e�N���X�ɓo�^
	 * @return	�Ȃ�				*/
	void Init(GameObject* monster) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void)  override;

	/* @brief	���s���鏀��
	 * @param	�Ȃ�
	 * @return	�Ȃ�
	 * @detail	���̍U�����L�̃A�j���[�V�����A���x��ݒ肷��		*/
	void SetMove(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�U�����I��������true
	 * @detail	�p���ύX�͂Ȃ��A�ړ����Ȃ��悤���x��0�ɌŒ�
				��莞�Ԍo�ߌ�A�A�j���[�V�����̑��x���グ��A�A�j���[�V�������I�������猳�ɖ߂��ďI��		*/
	bool Update(void)  override;
	
	/* @brief	���[�V�����̏I������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void EndMove(void) override;
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�
	 * @detail	�U�����I�u�W�F�N�g����Ă΂��		*/
	void GuiUpdate(void) override;

private:
	//! �����蔻��
	Collider3D::OBB* collider_;
};

#endif // _DRAGON_BITE_H_
