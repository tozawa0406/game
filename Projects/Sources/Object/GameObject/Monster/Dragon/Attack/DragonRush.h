/*
 * @file		DragonRush.h
 * @brief		—´‚Ì“Ëi
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/23
 */
#ifndef _DRAGON_RUSH_H_
#define _DRAGON_RUSH_H_

#include "../../MonsterAttack.h"

class DragonRush : public MonsterAttack
{
public:
	DragonRush(void);
	~DragonRush(void);

	void Init(GameObject* monster) override;
	void Uninit(void)  override;
	void SetMove(void) override;
	bool Update(void)  override;
	void EndMove(void) override;

	void GuiUpdate(void) override;

private:
	int cnt_;
};

#endif // _DRAGON_RUSH_H_
