/*
 * @file		SetupAttackState.h
 * @brief		�[��������
 * @author		���V�đ�
 * @data		2018/11/24
 */
#ifndef _SETUP_ATTACK_STATE_H_
#define _SETUP_ATTACK_STATE_H_

#include "AttackBaseState.h"

class SetupAttackState : public AttackBaseState
{
public:
	/* @brief	�R���X�g���N�^		*/
	SetupAttackState(void);
	/* @brief	�f�X�g���N�^		*/
	~SetupAttackState(void);


	
	/* @brief	������
	 * @param	(player)	�v���C���[�̃|�C���^
	 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
	 * @return	�Ȃ�				*/
	void Init(Player* player, Controller* ctrl) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	���̃X�e�[�g		*/
	PlayerState* Update(void) override;
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�
	 * @detail	�v���C���[����Ă΂��	*/
	void GuiUpdate(void) override;

private:
	bool isDraw_;		//! �[���������
};

#endif // _SETUP_ATTACK_STATE_H_
