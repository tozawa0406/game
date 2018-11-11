/*
 * @file		PlayerState.h
 * @brief		プレイヤーの行動
 * @author		戸澤翔太
 * @data		2018/11/10
 */
#ifndef _PLAYER_STATE_H_
#define _PLAYER_STATE_H_

#include <FrameWork/Systems/Renderer/Model/MeshRenderer.h>
#include <FrameWork/Systems/Input/Controller.h>
#include "../../Wapon/Wapon.h"
#include "../PlayerHunter.h"

class PlayerState
{
public:
	PlayerState(void) {}
	virtual ~PlayerState(void) {};

	virtual void Init(PlayerHunter* player, Controller* ctrl) { player_ = player; ctrl_ = ctrl; }
	virtual void Uninit(void) = 0;
	virtual PlayerState* Update(void) = 0;

protected:
	PlayerHunter*	player_;
	Controller*		ctrl_;
};

#endif // _PLAYER_STATE_H_
