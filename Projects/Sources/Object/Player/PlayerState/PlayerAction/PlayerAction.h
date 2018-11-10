/*
 * @file		PlayerAction.h
 * @brief		プレイヤーの行動
 * @author		戸澤翔太
 * @data		2018/11/10
 */
#ifndef _PLAYER_ACTION_H_
#define _PLAYER_ACTION_H_

#include <FrameWork/Systems/Renderer/Model/MeshRenderer.h>
#include <FrameWork/Systems/Input/Controller.h>
#include "../../../Wapon/Wapon.h"
#include "../../PlayerHunter.h"

class PlayerAction
{
public:
	PlayerAction(void) {}
	virtual ~PlayerAction(void) {};

	virtual void Init(PlayerHunter* player, Controller* ctrl) { player_ = player; ctrl_ = ctrl; }
	virtual void Uninit(void) = 0;
	virtual PlayerAction* Update(void) = 0;

protected:
	PlayerHunter*	player_;
	Controller*		ctrl_;
};

#endif // _PLAYER_ACTION_H_
