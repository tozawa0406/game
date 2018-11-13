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
#include "../../../Wapon/Wapon.h"
#include "../Player.h"

class PlayerState
{
public:
	//! @def	移動速度
	static constexpr float MOVE_SPEED = 0.06f;
	//! @def	アニメーション変更速度
	static constexpr int   ANIMATION_CHANGE_FRAME15 = 15;
	//! @def	アニメーション変更速度
	static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;
	//! @def	アニメーションの速度
	static constexpr float ANIMATION_DEFAULT = 0.75f;


	PlayerState(void) {}
	virtual ~PlayerState(void) {};

	virtual void Init(Player* player, Controller* ctrl) { player_ = player; ctrl_ = ctrl; }
	virtual void Uninit(void) = 0;
	virtual PlayerState* Update(void) = 0;

	virtual void GuiUpdate(void) = 0;

protected:
	Player*			player_;
	Controller*		ctrl_;
};

#endif // _PLAYER_STATE_H_
