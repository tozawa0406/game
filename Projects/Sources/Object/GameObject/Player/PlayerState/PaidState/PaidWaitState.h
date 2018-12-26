/*
 * @file		PaidWaitState.h
 * @brief		納刀の待機処理
 * @author		戸澤翔太
 * @data		2018/11/10
 */
#ifndef _PAID_WAIT_STATE_H_
#define _PAID_WAIT_STATE_H_

#include "../PlayerState.h"

class PaidWaitState : public PlayerState
{
public:
	/* @brief	コンストラクタ		*/
	PaidWaitState(void);
	/* @brief	デストラクタ		*/
	~PaidWaitState(void);


	
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
	 * @detail	プレイヤーから呼び出される	*/
	void GuiUpdate(void) override;

private:
	int waitCnt_;		//! 待ち時間
};

#endif // _PAID_WAIT_STATE_H_
