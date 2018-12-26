/*
 * @file		EmergencyAvoidanceState.h
 * @brief		�ً}�������
 * @author		���V�đ�
 * @data		2018/11/10
 */
#ifndef _EMERGENCY_AVOIDANCE_STATE_H_
#define _EMERGENCY_AVOIDANCE_STATE_H_

#include "PlayerState.h"

class EmergencyAvoidanceState : public PlayerState
{
public:
	//! @def	�g�p���ɏ����X�^�~�i
	static constexpr int DEC_STAMINA = 30;
	
	/* @brief	�R���X�g���N�^		*/
	EmergencyAvoidanceState(void);
	/* @brief	�f�X�g���N�^		*/
	~EmergencyAvoidanceState(void);


	
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

private:
	VECTOR3 dir_;		//! ����
	int		cnt_;		//! �J�E���^
};

#endif // _EMERGENCY_AVOIDANCE_STATE_H_
