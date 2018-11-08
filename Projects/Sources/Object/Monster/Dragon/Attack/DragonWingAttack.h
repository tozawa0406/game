/*
 * @file		DragonWingAttack.h
 * @brief		īĖU
 * @author		ËāVãÄū
 * @data		2018/11/07
 */
#ifndef _DRAGON_WING_ATTACK_H_
#define _DRAGON_WING_ATTACK_H_

#include "../../MonsterAttack.h"

class DragonWingAttack : public MonsterAttack
{
	//! @enum	
	enum class Wing : uint8
	{
		CLAW_L = 0,
		CLAW_R,

		MAX
	};
public:
	DragonWingAttack(void);
	~DragonWingAttack(void);

	void Init(Object* object) override;
	void Uninit(void)         override;
	void SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum) override;
	bool Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd) override;

	void GuiUpdate(void) override;

private:
	//! Ė―čŧč
	Collider3D::OBB* collider_[static_cast<int>(Wing::MAX)];

	//! fboO@\Ågp
	float debug_speed_;
	int   debug_changeFrame_;
};

#endif // _DRAGON_WING_ATTACK_H_
