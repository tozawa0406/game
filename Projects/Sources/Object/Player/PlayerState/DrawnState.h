/*
 * @file		DrawnState.h
 * @brief		”²“ó‘Ô
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/10
 */
#ifndef _DRAWN_STATE_H_
#define _DRAWN_STATE_H_

#include "PlayerState.h"

class DrawnState : public PlayerState
{
public:
	DrawnState(void);
	~DrawnState(void);

	PlayerState* Setup(void) override;
	void Move(void) override;
	void Attack(void) override;
	void Avoidance(void) override;
};

#endif // _DRAWN_STATE_H_
