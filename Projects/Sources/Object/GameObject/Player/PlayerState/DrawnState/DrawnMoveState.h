/*
 * @file		DrawnMoveState.h
 * @brief		������Ԃ̈ړ�����
 * @author		���V�đ�
 * @data		2018/11/10
 */
#ifndef _DRAWN_MOVE_STATE_H_
#define _DRAWN_MOVE_STATE_H_

#include "../PlayerState.h"

class DrawnMoveState : public PlayerState
{
public:
	/* @brief	�R���X�g���N�^		*/
	DrawnMoveState(void);
	/* @brief	�f�X�g���N�^		*/
	~DrawnMoveState(void);


	
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

#endif // _DRAWN_MOVE_STATE_H_
