/*
 * @file		KnockBackState.h
 * @brief		�]�я���
 * @author		���V�đ�
 * @data		2018/11/12
 */
#ifndef _KNOCK_BACK_STATE_H_
#define _KNOCK_BACK_STATE_H_

#include "PlayerState.h"

class KnockBackState : public PlayerState
{
public:
	/* @brief	�R���X�g���N�^		*/
	KnockBackState(void);
	/* @brief	�f�X�g���N�^		*/
	~KnockBackState(void);


	
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
};

#endif // _KNOCK_BACK_STATE_H_
