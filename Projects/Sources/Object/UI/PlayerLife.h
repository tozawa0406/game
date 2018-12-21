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
	/* @brief	コンストラクタ		*/
	PlayerLife(void);
	/* @brief	デストラクタ		*/
	~PlayerLife(void);


	
	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void)   override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;

	/* @brief	更新処理
	 * @param	なし
	 * @return	なし				*/
	void Update(void) override;

	/* @brief	プレイヤーの設定
	 * @param	(player)	プレイヤー		*/
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
