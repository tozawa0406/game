/*
 * @file		DragonBite.h
 * @brief		龍の噛みつき攻撃
 * @author		戸澤翔太
 * @data		2018/11/07
 */
#ifndef _DRAGON_BITE_H_
#define _DRAGON_BITE_H_

#include "MonsterAttack.h"

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
#ifdef _SELF_DEBUG
	//! デッバグ機能で使用
	float debug_speed_;			//*****使用の際には絶対に#ifdefで囲む*****
	int   debug_changeFrame_;	//*****使用の際には絶対に#ifdefで囲む*****
#endif
};

#endif // _DRAGON_BITE_H_
