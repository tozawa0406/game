/*
 * @file		DragonScream.h
 * @brief		—´‚Ì™ôšK
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/06
 */
#ifndef _DRAGON_SCREAM_H_
#define _DRAGON_SCREAM_H_

#include "../../MonsterAttack.h"

class DragonScream : public MonsterAttack
{
public:
	DragonScream(void);
	~DragonScream(void);

	void Init(Object* object) override;
	void Uninit(void)         override;
	void SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum) override;
	bool Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd) override;

private:
};

#endif // _DRAGON_SCREAM_H_
