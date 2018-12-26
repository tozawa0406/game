/*
 * @file		DragonRush.h
 * @brief		���̓ːi
 * @author		���V�đ�
 * @data		2018/11/23
 */
#ifndef _DRAGON_RUSH_H_
#define _DRAGON_RUSH_H_

#include "../../MonsterAttack.h"

class DragonRush : public MonsterAttack
{
public:
	/* @brief	�R���X�g���N�^		*/
	DragonRush(void);
	/* @brief	�f�X�g���N�^		*/
	~DragonRush(void);
	
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
	int cnt_;		//! �J�E���^
};

#endif // _DRAGON_RUSH_H_
