/*
 * @file		DrawnMoveState.h
 * @brief		抜刀状態の移動処理
 * @author		戸澤翔太
 * @data		2018/11/10
 */
#ifndef _DRAWN_MOVE_STATE_H_
#define _DRAWN_MOVE_STATE_H_

#include "../PlayerState.h"

class DrawnMoveState : public PlayerState
{
public:
	/* @brief	コンストラクタ		*/
	DrawnMoveState(void);
	/* @brief	デストラクタ		*/
	~DrawnMoveState(void);


	
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
	void GuiUpdate(void) override;
};

#endif // _DRAWN_MOVE_STATE_H_
