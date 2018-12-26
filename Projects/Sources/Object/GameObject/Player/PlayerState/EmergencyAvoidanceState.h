/*
 * @file		EmergencyAvoidanceState.h
 * @brief		緊急回避処理
 * @author		戸澤翔太
 * @data		2018/11/10
 */
#ifndef _EMERGENCY_AVOIDANCE_STATE_H_
#define _EMERGENCY_AVOIDANCE_STATE_H_

#include "PlayerState.h"

class EmergencyAvoidanceState : public PlayerState
{
public:
	//! @def	使用時に消費するスタミナ
	static constexpr int DEC_STAMINA = 30;
	
	/* @brief	コンストラクタ		*/
	EmergencyAvoidanceState(void);
	/* @brief	デストラクタ		*/
	~EmergencyAvoidanceState(void);


	
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

private:
	VECTOR3 dir_;		//! 方向
	int		cnt_;		//! カウンタ
};

#endif // _EMERGENCY_AVOIDANCE_STATE_H_
