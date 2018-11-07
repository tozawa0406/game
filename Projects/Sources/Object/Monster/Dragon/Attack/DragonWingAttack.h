/*
 * @file		DragonWingAttack.h
 * @brief		龍の翼攻撃
 * @author		戸澤翔太
 * @data		2018/11/07
 */
#ifndef _DRAGON_WING_ATTACK_H_
#define _DRAGON_WING_ATTACK_H_

#include "../../MonsterAttack.h"

class DragonWingAttack : public MonsterAttack
{
public:
	DragonWingAttack(void);
	~DragonWingAttack(void);

	void Init(Object* object) override;
	void Uninit(void)         override;
	void SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum) override;
	bool Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd) override;

	void GuiUpdate(void) override;

private:
	//! デッバグ機能で使用
	float debug_speed_;
	int   debug_changeFrame_;
};

#endif // _DRAGON_WING_ATTACK_H_
