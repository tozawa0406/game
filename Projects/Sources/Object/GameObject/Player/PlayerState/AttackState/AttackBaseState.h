/*
 * @file		AttackBaseState.h
 * @brief		UŒ‚ˆ—‚ÌŠî’êƒNƒ‰ƒX
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/26
 */
#ifndef _ATTACK_BASE_STATE_H_
#define _ATTACK_BASE_STATE_H_

#include "../PlayerState.h"

class AttackBaseState : public PlayerState
{
public:
	AttackBaseState(void);
	virtual ~AttackBaseState(void);

	virtual void Init(Player* player, Controller* ctrl) override;
	virtual void Uninit(void) override;
	virtual PlayerState* Update(void) override;

	virtual void GuiUpdate(void) override;

protected:
	float animSpeed_;
	Player::Animation animation_;
	int changeFrame_;

	int collisionStart_;
	int collisionEnd_;

	int moveStart_;
	int moveEnd_;

	float move_;

	bool next_;		//! Ÿ‚ÌUŒ‚‚Ì“ü—Í
	VECTOR3 dir_;

private:
	bool debug_nextFrame_;
};

#endif // _ATTACK_BASE_STATE_H_
