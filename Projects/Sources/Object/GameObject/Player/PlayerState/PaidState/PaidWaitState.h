/*
 * @file		PaidWaitState.h
 * @brief		�[���̑ҋ@����
 * @author		���V�đ�
 * @data		2018/11/10
 */
#ifndef _PAID_WAIT_STATE_H_
#define _PAID_WAIT_STATE_H_

#include "../PlayerState.h"

class PaidWaitState : public PlayerState
{
public:
	/* @brief	�R���X�g���N�^		*/
	PaidWaitState(void);
	/* @brief	�f�X�g���N�^		*/
	~PaidWaitState(void);


	
	/* @brief	����������
	 * @param	(player)	�v���C���[�ւ̃|�C���^
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
	 * @detail	�v���C���[����Ăяo�����	*/
	void GuiUpdate(void) override;

private:
	int waitCnt_;		//! �҂�����
};

#endif // _PAID_WAIT_STATE_H_
