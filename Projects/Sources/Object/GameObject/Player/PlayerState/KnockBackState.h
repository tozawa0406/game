/*
 * @file		KnockBackState.h
 * @brief		“]‚Ñˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/12
 */
#ifndef _KNOCK_BACK_STATE_H_
#define _KNOCK_BACK_STATE_H_

#include "PlayerState.h"

class KnockBackState : public PlayerState
{
public:
	KnockBackState(void);
	~KnockBackState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;
};

#endif // _KNOCK_BACK_STATE_H_
