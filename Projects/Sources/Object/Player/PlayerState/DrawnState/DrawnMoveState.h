/*
 * @file		DrawnMoveState.h
 * @brief		”²“ó‘Ô‚ÌˆÚ“®ˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/10
 */
#ifndef _DRAWN_MOVE_STATE_H_
#define _DRAWN_MOVE_STATE_H_

#include "../PlayerState.h"

class DrawnMoveState : public PlayerState
{
public:
	DrawnMoveState(void);
	~DrawnMoveState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;
};

#endif // _DRAWN_MOVE_STATE_H_
