/*
 * @file		DieState.h
 * @brief		���S����
 * @author		���V�đ�
 * @data		2018/11/14
 */
#ifndef _DIE_STATE_H_
#define _DIE_STATE_H_

#include "PlayerState.h"

class DieState : public PlayerState
{
public:
	/* @brief	�R���X�g���N�^		*/
	DieState(void);
	/* @brief	�f�X�g���N�^		*/
	~DieState(void);


	
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
	 * @detail	�v���C���[����Ă΂��	*/
	void GuiUpdate(void) override;
};

#endif // _DIE_STATE_H_
