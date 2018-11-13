/*
 * @file		DrawnWaitState.h
 * @brief		”²“‚Ì‘Ò‹@ˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/10
 */
#ifndef _DRAWN_WAIT_STATE_H_
#define _DRAWN_WAIT_STATE_H_

#include "../PlayerState.h"

class DrawnWaitState : public PlayerState
{
public:
	DrawnWaitState(void);
	~DrawnWaitState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;
};

#endif // _DRAWN_WAIT_STATE_H_
