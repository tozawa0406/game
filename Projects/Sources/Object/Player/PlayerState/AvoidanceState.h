/*
 * @file		AvoidanceState.h
 * @brief		‰ñ”ğˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/10
 */
#ifndef _AVOIDANCE_STATE_H_
#define _AVOIDANCE_STATE_H_

#include "PlayerState.h"

class AvoidanceState : public PlayerState
{
public:
	AvoidanceState(void);
	~AvoidanceState(void);

	void Init(PlayerHunter* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

private:
	VECTOR3 dir_;
	bool	isDraw_;
};

#endif // _AVOIDANCE_ACTION_H_
