/*
 * @file		Slash3AttackState.h
 * @brief		�U������
 * @author		���V�đ�
 * @data		2018/11/24
 */
#ifndef _SLASH3_ATTACK_STATE_H_
#define _SLASH3_ATTACK_STATE_H_

#include "AttackBaseState.h"

class Slash3AttackState : public AttackBaseState
{
public:
	Slash3AttackState(void);
	~Slash3AttackState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;
};

#endif // _SLASH3_ATTACK_STATE_H_
