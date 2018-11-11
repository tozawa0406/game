/*
 * @file		PaidMoveState.h
 * @brief		�[����Ԃ̈ړ�����
 * @author		���V�đ�
 * @data		2018/11/10
 */
#ifndef _PAID_MOVE_STATE_H_
#define _PAID_MOVE_STATE_H_

#include "../PlayerState.h"

class PaidMoveState : public PlayerState
{
public:
	PaidMoveState(void);
	~PaidMoveState(void);

	void Init(PlayerHunter* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

private:
	VECTOR2 inputDir_;
	float   inputDash_;
};

#endif // _PAID_MOVE_ACTION_H_
