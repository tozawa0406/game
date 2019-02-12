/*
 * @file		AttackBaseState.h
 * @brief		�U�������̊��N���X
 * @author		���V�đ�
 * @data		2018/11/26
 */
#ifndef _ATTACK_BASE_STATE_H_
#define _ATTACK_BASE_STATE_H_

#include "../PlayerState.h"

class AttackBaseState : public PlayerState
{
public:
	/* @brief	�R���X�g���N�^		*/
	AttackBaseState(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~AttackBaseState(void);



	/* @brief	����������
	 * @param	(player)	�v���C���[�ւ̃|�C���^
	 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
	 * @return	�Ȃ�				*/
	virtual void Init(Player* player, Controller* ctrl) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	virtual void Uninit(void) override;
	
	/* @breif	�X�V����
	 * @param	�Ȃ�
	 * @return	���̃X�e�[�g		*/
	virtual PlayerState* Update(void) override;
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�
	 * @detail	�v���C���[����Ă΂��	*/
	virtual void GuiUpdate(void) override;

protected:
	//! @def	�A�j���[�V�������x
	static constexpr float ANIM_SPEED = 0.75f;

	Player::Animation	animation_;		//! �A�j���[�V����
	float				animSpeed_;		//! �A�j���[�V�������x
	int					changeFrame_;	//! 

	int		collisionStart_;	//! �����蔻��̊J�n
	int		collisionEnd_;		//! �����蔻��̏I��

	float	effectRot_;		//! �q�b�g�G�t�F�N�g�̉�]�p�x
	int		moveStart_;		//! �����n��
	int		moveEnd_;		//! �����I���

	float	move_;		//! 1�t���[���œ�������

	int		voice_;		//! ��
	bool	next_;		//! ���̍U���̓���
	VECTOR3 dir_;		//! ����

private:
	bool debug_nextFrame_;	//!�f�o�b�O���̎��̃t���[��
};

#endif // _ATTACK_BASE_STATE_H_
