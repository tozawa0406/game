/*
 * @file		PaidState.h
 * @brief		”[“ó‘Ô
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/10
 */
#ifndef _PAID_STATE_H_
#define _PAID_STATE_H_

#include "PlayerState.h"

class PaidState : public PlayerState
{
public:
	PaidState(void);
	~PaidState(void);

	void Init(PlayerHunter* player, Controller* ctrl) override;
	void Update(void);
	PlayerState* Setup(void) override;
	void Move(void) override;
	void Attack(void) override;
	void Avoidance(void) override;
};

#endif // _PAID_STATE_H_
