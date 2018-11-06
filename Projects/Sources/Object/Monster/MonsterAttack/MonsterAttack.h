/*
 * @file		MonsterAttack.h
 * @brief		X^[ĖUNXĖîęNX
 * @author		ËāVãÄū
 * @data		2018/11/06
 */
#ifndef _MONSTER_ATTACK_H_
#define _MONSTER_ATTACK_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class MonsterAttack
{
public:
	MonsterAttack(void)	: enable_(false), frame_(0)	{}
	virtual ~MonsterAttack(void)	{}

	virtual void Init(Object* object) = 0;
	virtual void Uninit(void) = 0;
	virtual void SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum) { enable_ = true; frame_ = 0; UNREFERENCED_PARAMETER(mesh); UNREFERENCED_PARAMETER(animNum); UNREFERENCED_PARAMETER(animSpeed); }
	virtual bool Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd) = 0;

protected:
	//! gptO
	bool	enable_;
	//! UĖ―čŧčŧfp
	int		frame_;
};

#endif // _MONSTAER_ATTACK_H_
