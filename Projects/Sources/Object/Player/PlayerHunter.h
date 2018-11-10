/*
 * @file		PlayerHunter.h
 * @brief		ƒvƒŒƒCƒ„[‚Ì“®‚«
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/02
 */
#ifndef _PLAYER_HUNTER_H_
#define _PLAYER_HUNTER_H_

#include "PlayerMove.h"

class PlayerHunter : public PlayerMove
{
public:
	PlayerHunter(void);
	~PlayerHunter(void);

	void Init(void)   override;
	void Uninit(void) override;
	void Update(void) override;

	void GuiUpdate(void) override;

private:
	void Setup(void);
	void Attack(void);

};

#endif // _PLAYER_HUNTER_H_
