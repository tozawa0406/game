/*
 * @file		DragonTakeOff.h
 * @brief		—´‚Ì—ƒUŒ‚
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/07
 */
#ifndef _DRAGON_TAKE_OFF_H_
#define _DRAGON_TAKE_OFF_H_

#include "../../MonsterAttack.h"

class DragonTakeOff : public MonsterAttack
{
public:
	DragonTakeOff(void);
	~DragonTakeOff(void);

	void Init(GameObject* monster) override;
	void Uninit(void)  override;
	void SetMove(void) override;
	bool Update(void)  override;

	inline void SetFly(bool fly) { fly_ = fly; }

	void GuiUpdate(void) override;

private:
	bool fly_;

	float debug_velocity_;
};

#endif // _DRAGON_TAKE_OFF_H_
