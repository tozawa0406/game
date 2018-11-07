/*
 * @file		DragonTakeOff.h
 * @brief		´ĖU
 * @author		ËāVãÄž
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

	void Init(Object* object) override;
	void Uninit(void)         override;
	void SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum) override;
	bool Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd) override;

	inline void SetFly(bool fly) { fly_ = fly; }

	void GuiUpdate(void) override;

private:
	bool fly_;

	float debug_velocity_;
};

#endif // _DRAGON_TAKE_OFF_H_
