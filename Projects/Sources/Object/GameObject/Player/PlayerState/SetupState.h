/*
 * @file		SetupState.h
 * @brief		�[��������
 * @author		���V�đ�
 * @data		2018/11/11
 */
#ifndef _SETUP_STATE_H_
#define _SETUP_STATE_H_

#include "PlayerState.h"

class SetupState : public PlayerState
{
public:
	/* @brief	�R���X�g���N�^		*/
	SetupState(void);
	/* @brief	�f�X�g���N�^		*/
	~SetupState(void);



	/* @brief	����������
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
	bool isDraw_;	//! �����t���O
};

#endif // _SETUP_STATE_H_
