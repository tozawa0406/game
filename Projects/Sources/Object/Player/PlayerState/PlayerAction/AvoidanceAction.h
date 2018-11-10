/*
 * @file		AvoidanceAction.h
 * @brief		‰ñ”ğˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/10
 */
#ifndef _AVOIDANCE_ACTION_H_
#define _AVOIDANCE_ACTION_H_

#include "PlayerAction.h"

class AvoidanceAction : public PlayerAction
{
public:
	AvoidanceAction(void);
	~AvoidanceAction(void);

	void Init(PlayerHunter* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerAction* Update(void) override;

private:
	VECTOR3 dir_;
};

#endif // _AVOIDANCE_ACTION_H_
