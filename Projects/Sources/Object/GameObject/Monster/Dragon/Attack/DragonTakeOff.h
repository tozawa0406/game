/*
 * @file		DragonTakeOff.h
 * @brief		īĖU
 * @author		ËāVãÄū
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
	void EndMove(void) override;

	inline void SetFly(bool fly) { fly_ = fly; }

	void GuiUpdate(void) override;

private:
	bool fly_;		//! ōņÅĒétO

	float debug_velocity_;
};

#endif // _DRAGON_TAKE_OFF_H_
