/*
 * @file		PlayerLife.h
 * @brief		プレイヤーのライフとスタミナ
 * @author		戸澤翔太
 * @data		2018/11/19
 */
#ifndef _PLAYER_LIFE_H_
#define _PLAYER_LIFE_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

#include "../GameObject/Player/Player.h"

class PlayerLife : public Object
{
public:
	PlayerLife(void);
	~PlayerLife(void);

	void Init(void)   override;
	void Uninit(void) override;
	void Update(void) override;

	inline void SetPlayer(Player* player) { player_ = player; }

private:
	//! プレイヤーのポインタ
	Player* player_;

	//! 背景
	CanvasRenderer::Image back_;
	//! ライフバー
	CanvasRenderer::Image life_;
	//! スタミナバー
	CanvasRenderer::Image stamina_;
};

#endif // _PLAYER_LIFE_H_
