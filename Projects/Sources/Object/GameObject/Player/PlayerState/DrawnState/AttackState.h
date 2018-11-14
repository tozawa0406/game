/*
 * @file		AttackState.h
 * @brief		UŒ‚ˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/12
 */
#ifndef _ATTACK_STATE_H_
#define _ATTACK_STATE_H_

#include "../PlayerState.h"

class AttackState : public PlayerState
{
public:
	AttackState(void);
	~AttackState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;

private:
	bool next_;		//! Ÿ‚ÌUŒ‚‚Ì“ü—Í
	int frame_;
	bool debug_nextFrame_;
};

#endif // _ATTACK_STATE_H_
