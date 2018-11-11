/*
 * @file		PaidWaitState.h
 * @brief		”[“‚Ì‘Ò‹@ˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/10
 */
#ifndef _PAID_WAIT_STATE_H_
#define _PAID_WAIT_STATE_H_

#include "../PlayerState.h"

class PaidWaitState : public PlayerState
{
public:
	PaidWaitState(void);
	~PaidWaitState(void);

	void Init(PlayerHunter* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

private:
	int waitCnt_;
};

#endif // _PAID_MOVE_STATE_H_
