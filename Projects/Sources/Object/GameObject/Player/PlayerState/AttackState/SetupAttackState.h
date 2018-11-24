/*
 * @file		SetupAttackState.h
 * @brief		[ē
 * @author		ËāVãÄū
 * @data		2018/11/24
 */
#ifndef _SETUP_ATTACK_STATE_H_
#define _SETUP_ATTACK_STATE_H_

#include "../PlayerState.h"

class SetupAttackState : public PlayerState
{
public:
	SetupAttackState(void);
	~SetupAttackState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;

private:
	bool isDraw_;
	bool next_;		//! ĖUĖüÍ
	bool debug_nextFrame_;
};

#endif // _SETUP_ATTACK_STATE_H_
