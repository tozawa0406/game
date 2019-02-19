/*
 * @file		HealState.h
 * @brief		�񕜏���
 * @author		���V�đ�
 * @data		2019/01/26
 */
#ifndef _HEAL_STATE_H_
#define _HEAL_STATE_H_

#include "../PlayerState.h"

class HealState : public PlayerState
{
public:
	/* @brief	�R���X�g���N�^		*/
	HealState(void);
	/* @brief	�f�X�g���N�^		*/
	~HealState(void);


	
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
	//! �l�v�Z�ς݃t���O
	bool	heal_;
	//! �A�C�e���̎��ʎq
	ItemID	itemId_;
	//! ���ݍ��݉�
	bool	drink_;
};

#endif // _HEAL_STATE_H_
