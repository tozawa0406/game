/*
 * @file		DragonTakeOff.h
 * @brief		���̗��U��
 * @author		���V�đ�
 * @data		2018/11/07
 */
#ifndef _DRAGON_TAKE_OFF_H_
#define _DRAGON_TAKE_OFF_H_

#include "../../MonsterAttack.h"

class DragonTakeOff : public MonsterAttack
{
public:
	/* @brief	�R���X�g���N�^		*/
	DragonTakeOff(void);
	/* @brief	�R���X�g���N�^		*/
	~DragonTakeOff(void);
	
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

	/* @brief	��s��ԑJ��		*/
	inline void SetFly(bool fly) { fly_ = fly; }

private:
	bool fly_;		//! ���ł���t���O

	float debug_velocity_;
};

#endif // _DRAGON_TAKE_OFF_H_
