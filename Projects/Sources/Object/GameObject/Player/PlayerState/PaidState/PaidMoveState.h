/*
 * @file		PaidMoveState.h
 * @brief		納刀状態の移動処理
 * @author		戸澤翔太
 * @data		2018/11/10
 */
#ifndef _PAID_MOVE_STATE_H_
#define _PAID_MOVE_STATE_H_

#include "../PlayerState.h"

class PaidMoveState : public PlayerState
{
public:
	/* @brief	コンストラクタ		*/
	PaidMoveState(void);
	/* @brief	デストラクタ		*/
	~PaidMoveState(void);


	
	/* @brief	初期化処理
	 * @param	(player)	プレイヤーへのポインタ
	 * @param	(ctrl)		コントローラへのポインタ
	 * @return	なし				*/
	void Init(Player* player, Controller* ctrl) override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	次のステート		*/
	PlayerState* Update(void) override;
	
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし
	 * @detail	プレイヤーから呼ばれる	*/
	void GuiUpdate(void)override;

private:
	//! 走り判定
	bool dash_;
	//! 息切れ判定
	bool breath_;
};

#endif // _PAID_MOVE_STATE_H_
