/*
 * @file		PlayerState.h
 * @brief		ÉvÉåÉCÉÑÅ[ÇÃèÛë‘ä«óù
 * @author		åÀ‡V„ƒëæ
 * @data		2018/11/10
 */
#ifndef _PLAYER_STATE_H_
#define _PLAYER_STATE_H_

#include <FrameWork/Systems/Renderer/Model/MeshRenderer.h>
#include <FrameWork/Systems/Input/Controller.h>
#include "../../Wapon/Wapon.h"
#include "PlayerAction/PlayerAction.h"

class PlayerHunter;
class PlayerState
{
public:
	PlayerState(void) : action_(nullptr), player_(nullptr), ctrl_(nullptr) {}
	virtual ~PlayerState(void) {};

	void Uninit(void) { DeletePtr(action_); }

	virtual void Init(PlayerHunter* player, Controller* ctrl) { player_ = player; ctrl_ = ctrl; }
	virtual void Update(void) = 0;

	virtual PlayerState* Setup(void) = 0;
	virtual void Move(void)  = 0;
	virtual void Attack(void) = 0;
	virtual void Avoidance(void) = 0;

protected:
	PlayerAction*	action_;
	PlayerHunter*	player_;
	Controller*		ctrl_;
};

#endif // _PLAYER_STATE_H_
