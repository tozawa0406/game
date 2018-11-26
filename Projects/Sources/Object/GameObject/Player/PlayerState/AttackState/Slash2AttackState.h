/*
 * @file		Slash2AttackState.h
 * @brief		UŒ‚ˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/24
 */
#ifndef _SLASH2_ATTACK_STATE_H_
#define _SLASH2_ATTACK_STATE_H_

#include "AttackBaseState.h"

class Slash2AttackState : public AttackBaseState
{
public:
	Slash2AttackState(void);
	~Slash2AttackState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;
};

#endif // _SLASH2_ATTACK_STATE_H_
