/*
 * @file		Slash1AttackState.h
 * @brief		U
 * @author		ËāVãÄū
 * @data		2018/11/24
 */
#ifndef _SLASH1_ATTACK_STATE_H_
#define _SLASH1_ATTACK_STATE_H_

#include "../PlayerState.h"

class Slash1AttackState : public PlayerState
{
public:
	Slash1AttackState(void);
	~Slash1AttackState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;

private:
	bool next_;		//! ĖUĖüÍ
	bool debug_nextFrame_;
};

#endif // _SLASH1_ATTACK_STATE_H_
