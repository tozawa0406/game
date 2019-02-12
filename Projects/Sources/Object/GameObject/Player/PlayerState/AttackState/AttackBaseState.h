/*
 * @file		AttackBaseState.h
 * @brief		攻撃処理の基底クラス
 * @author		戸澤翔太
 * @data		2018/11/26
 */
#ifndef _ATTACK_BASE_STATE_H_
#define _ATTACK_BASE_STATE_H_

#include "../PlayerState.h"

class AttackBaseState : public PlayerState
{
public:
	/* @brief	コンストラクタ		*/
	AttackBaseState(void);
	/* @brief	デストラクタ		*/
	virtual ~AttackBaseState(void);



	/* @brief	初期化処理
	 * @param	(player)	プレイヤーへのポインタ
	 * @param	(ctrl)		コントローラへのポインタ
	 * @return	なし				*/
	virtual void Init(Player* player, Controller* ctrl) override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	virtual void Uninit(void) override;
	
	/* @breif	更新処理
	 * @param	なし
	 * @return	次のステート		*/
	virtual PlayerState* Update(void) override;
	
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし
	 * @detail	プレイヤーから呼ばれる	*/
	virtual void GuiUpdate(void) override;

protected:
	//! @def	アニメーション速度
	static constexpr float ANIM_SPEED = 0.75f;

	Player::Animation	animation_;		//! アニメーション
	float				animSpeed_;		//! アニメーション速度
	int					changeFrame_;	//! 

	int		collisionStart_;	//! 当たり判定の開始
	int		collisionEnd_;		//! 当たり判定の終了

	float	effectRot_;		//! ヒットエフェクトの回転角度
	int		moveStart_;		//! 動き始め
	int		moveEnd_;		//! 動き終わり

	float	move_;		//! 1フレームで動く距離

	int		voice_;		//! 声
	bool	next_;		//! 次の攻撃の入力
	VECTOR3 dir_;		//! 方向

private:
	bool debug_nextFrame_;	//!デバッグ中の次のフレーム
};

#endif // _ATTACK_BASE_STATE_H_
