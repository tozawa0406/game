/*
 * @file		DrawnMoveAction.h
 * @brief		”²“ó‘Ô‚ÌˆÚ“®ˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/10
 */
#ifndef _DRAWN_MOVE_ACTION_H_
#define _DRAWN_MOVE_ACTION_H_

#include "../PlayerAction.h"

class DrawnMoveAction : public PlayerAction
{
public:
	DrawnMoveAction(void);
	~DrawnMoveAction(void);

	void Init(PlayerHunter* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerAction* Update(void) override;

private:
	VECTOR2 inputDir_;
};

#endif // _DRAWN_MOVE_ACTION_H_
