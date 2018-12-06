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
	AttackBaseState(void);
	virtual ~AttackBaseState(void);

	virtual void Init(Player* player, Controller* ctrl) override;
	virtual void Uninit(void) override;
	virtual PlayerState* Update(void) override;

	virtual void GuiUpdate(void) override;

protected:
	static constexpr float ANIM_SPEED = 0.75f;

	Player::Animation	animation_;		//! �A�j���[�V����
	float				animSpeed_;		//! �A�j���[�V�������x
	int					changeFrame_;	//! 

	int		collisionStart_;	//! �����蔻��̊J�n
	int		collisionEnd_;		//! �����蔻��̏I��

	int		moveStart_;		//! �����n��
	int		moveEnd_;		//! �����I���

	float	move_;		//! 1�t���[���œ�������

	bool	next_;		//! ���̍U���̓���
	VECTOR3 dir_;		//! ����

private:
	bool debug_nextFrame_;	//!�f�o�b�O���̎��̃t���[��
};

#endif // _ATTACK_BASE_STATE_H_
