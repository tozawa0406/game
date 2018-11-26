/*
 * @file		Slash1AttackState.h
 * @brief		UŒ‚ˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/24
 */
#ifndef _SLASH1_ATTACK_STATE_H_
#define _SLASH1_ATTACK_STATE_H_

#include "AttackBaseState.h"

class Slash1AttackState : public AttackBaseState
{
public:
	Slash1AttackState(void);
	~Slash1AttackState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;
};

#endif // _SLASH1_ATTACK_STATE_H_
