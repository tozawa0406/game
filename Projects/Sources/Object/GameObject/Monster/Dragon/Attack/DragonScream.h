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

	void Init(GameObject* monster) override;
	void Uninit(void)  override;
	void SetMove(void) override;
	bool Update(void)  override;
};

#endif // _DRAGON_SCREAM_H_
