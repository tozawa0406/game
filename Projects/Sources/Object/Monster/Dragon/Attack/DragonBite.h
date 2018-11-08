/*
 * @file		DragonBite.h
 * @brief		īĖÝÂŦU
 * @author		ËāVãÄū
 * @data		2018/11/07
 */
#ifndef _DRAGON_BITE_H_
#define _DRAGON_BITE_H_

#include "../../MonsterAttack.h"

class DragonBite : public MonsterAttack
{
public:
	DragonBite(void);
	~DragonBite(void);

	void Init(Object* object) override;
	void Uninit(void)         override;
	void SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum) override;
	bool Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd) override;

	void GuiUpdate(void) override;

private:
	Collider3D::OBB* collision_;

	//! fboO@\Ågp
	float debug_speed_;
	int   debug_changeFrame_;
};

#endif // _DRAGON_BITE_H_
