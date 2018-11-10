/*
 * @file		PaidWait.h
 * @brief		”[“‚Ì‘Ò‹@ˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/10
 */
#ifndef _PAID_WAIT_ACTION_H_
#define _PAID_WAIT_ACTION_H_

#include "../PlayerAction.h"

class PaidWaitAction : public PlayerAction
{
public:
	PaidWaitAction(void);
	~PaidWaitAction(void);

	void Init(PlayerHunter* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerAction* Update(void) override;

private:
	int waitCnt_;
};

#endif // _PAID_MOVE_ACTION_H_
