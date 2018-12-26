/*
 * @file		SetupState.h
 * @brief		納抜刀処理
 * @author		戸澤翔太
 * @data		2018/11/11
 */
#ifndef _SETUP_STATE_H_
#define _SETUP_STATE_H_

#include "PlayerState.h"

class SetupState : public PlayerState
{
public:
	/* @brief	コンストラクタ		*/
	SetupState(void);
	/* @brief	デストラクタ		*/
	~SetupState(void);



	/* @brief	初期化処理
	 * @param	(player)	プレイヤーのポインタ
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

private:
	bool isDraw_;	//! 抜刀フラグ
};

#endif // _SETUP_STATE_H_
