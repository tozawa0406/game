/*
 * @file		HealState.h
 * @brief		回復処理
 * @author		戸澤翔太
 * @data		2019/01/26
 */
#ifndef _HEAL_STATE_H_
#define _HEAL_STATE_H_

#include "../PlayerState.h"

class HealState : public PlayerState
{
public:
	/* @brief	コンストラクタ		*/
	HealState(void);
	/* @brief	デストラクタ		*/
	~HealState(void);


	
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
	//! 値計算済みフラグ
	bool	heal_;
	//! アイテムの識別子
	ItemID	itemId_;
	//! 飲み込み音
	bool	drink_;
};

#endif // _HEAL_STATE_H_
