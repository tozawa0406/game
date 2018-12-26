/*
 * @file		SetupAttackState.h
 * @brief		納抜刀処理
 * @author		戸澤翔太
 * @data		2018/11/24
 */
#ifndef _SETUP_ATTACK_STATE_H_
#define _SETUP_ATTACK_STATE_H_

#include "AttackBaseState.h"

class SetupAttackState : public AttackBaseState
{
public:
	/* @brief	コンストラクタ		*/
	SetupAttackState(void);
	/* @brief	デストラクタ		*/
	~SetupAttackState(void);


	
	/* @brief	初期化
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
	bool isDraw_;		//! 納刀抜刀状態
};

#endif // _SETUP_ATTACK_STATE_H_
