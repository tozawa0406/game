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

class Player;
class PlayerLife : public Object
{
public:
	//! @def	最大値
	static constexpr float MAX_LIFE = 150;

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
	void MoveBar(CanvasRenderer::Image& bar, float value, const VECTOR2& offset, const float weight, CanvasRenderer::Image* ornament);

	//! プレイヤーのポインタ
	Player* player_;

	//! バーの両端の装飾
	CanvasRenderer::Image ornament_[4];
	//! ライフ背景
	CanvasRenderer::Image backLife_;
	//! スタミナ背景
	CanvasRenderer::Image backStamina_;
	//! ライフバー
	CanvasRenderer::Image life_;
	//! スタミナバー
	CanvasRenderer::Image stamina_;
};

#endif // _PLAYER_LIFE_H_
