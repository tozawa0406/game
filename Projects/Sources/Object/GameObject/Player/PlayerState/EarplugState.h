/*
 * @file		EarplugState.h
 * @brief		耳栓処理
 * @author		戸澤翔太
 * @data		2019/01/19
 */
#ifndef _EARPLUG_STATE_H_
#define _EARPLUG_STATE_H_

#include "PlayerState.h"

class EarplugState : public PlayerState
{
public:
	/* @brief	コンストラクタ		*/
	EarplugState(void);
	/* @brief	デストラクタ		*/
	~EarplugState(void);


	
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
};

#endif // _EARPLUG_STATE_H_
