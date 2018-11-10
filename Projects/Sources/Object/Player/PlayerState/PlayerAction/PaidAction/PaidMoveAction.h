/*
 * @file		PaidState.h
 * @brief		”[“ó‘Ô
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/10
 */
#ifndef _PAID_MOVE_ACTION_H_
#define _PAID_MOVE_ACTION_H_

#include "../PlayerAction.h"

class PaidMoveAction : public PlayerAction
{
public:
	PaidMoveAction(void);
	~PaidMoveAction(void);

	void Init(PlayerHunter* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerAction* Update(void) override;

private:
	VECTOR2 inputDir_;
	float   inputDash_;
};

#endif // _PAID_MOVE_ACTION_H_
