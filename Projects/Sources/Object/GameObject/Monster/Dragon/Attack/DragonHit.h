/*
 * @file		DragonHit.h
 * @brief		—´‚Ìƒ_ƒ[ƒWˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/06
 */
#ifndef _DRAGON_HIT_H_
#define _DRAGON_HIT_H_

#include "../../MonsterAttack.h"

class DragonHit : public MonsterAttack
{
public:
	DragonHit(void);
	~DragonHit(void);

	void Init(GameObject* monster) override;
	void Uninit(void)  override;
	void SetMove(void) override;
	bool Update(void)  override;

private:
};

#endif // _DRAGON_HIT_H_
