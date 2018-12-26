/*
 * @file		Slash1AttackState.h
 * @brief		攻撃処理
 * @author		戸澤翔太
 * @data		2018/11/24
 */
#ifndef _SLASH1_ATTACK_STATE_H_
#define _SLASH1_ATTACK_STATE_H_

#include "AttackBaseState.h"

class Slash1AttackState : public AttackBaseState
{
public:
	/* @brief	コンストラクタ		*/
	Slash1AttackState(void);
	/* @brief	デストラクタ		*/
	~Slash1AttackState(void);


	
	/* @brief	初期化処理
	 * @param	(player)	プレイヤーへのポインタ
	 * @param	(ctrl)		コントローラへのポインタ
	 * @return	なし				*/
	void Init(Player* player, Controller* ctrl) override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;
	
	/* @breif	更新処理
	 * @param	なし
	 * @return	次のステート		*/
	PlayerState* Update(void) override;
	
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし
	 * @detail	プレイヤーから呼ばれる	*/
	void GuiUpdate(void) override;
};

#endif // _SLASH1_ATTACK_STATE_H_
