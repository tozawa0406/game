/*
 * @file		PaidMoveState.h
 * @brief		”[“ó‘Ô‚ÌˆÚ“®ˆ—
 * @author		ŒËàVãÄ‘¾
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

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void)override;

private:
	bool dash_;
};

#endif // _PAID_MOVE_STATE_H_
