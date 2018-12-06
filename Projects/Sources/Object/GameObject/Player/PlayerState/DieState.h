/*
 * @file		DieState.h
 * @brief		€–Sˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/14
 */
#ifndef _DIE_STATE_H_
#define _DIE_STATE_H_

#include "PlayerState.h"

class DieState : public PlayerState
{
public:
	DieState(void);
	~DieState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;
};

#endif // _DIE_STATE_H_
