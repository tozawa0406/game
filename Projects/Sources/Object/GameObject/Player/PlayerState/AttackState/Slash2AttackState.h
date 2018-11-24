/*
 * @file		Slash2AttackState.h
 * @brief		�U������
 * @author		���V�đ�
 * @data		2018/11/24
 */
#ifndef _SLASH2_ATTACK_STATE_H_
#define _SLASH2_ATTACK_STATE_H_

#include "../PlayerState.h"

class Slash2AttackState : public PlayerState
{
public:
	Slash2AttackState(void);
	~Slash2AttackState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;

private:
	bool next_;		//! ���̍U���̓���
	bool debug_nextFrame_;
};

#endif // _SLASH2_ATTACK_STATE_H_
