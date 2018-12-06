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
	AttackBaseState(void);
	virtual ~AttackBaseState(void);

	virtual void Init(Player* player, Controller* ctrl) override;
	virtual void Uninit(void) override;
	virtual PlayerState* Update(void) override;

	virtual void GuiUpdate(void) override;

protected:
	static constexpr float ANIM_SPEED = 0.75f;

	Player::Animation	animation_;		//! アニメーション
	float				animSpeed_;		//! アニメーション速度
	int					changeFrame_;	//! 

	int		collisionStart_;	//! 当たり判定の開始
	int		collisionEnd_;		//! 当たり判定の終了

	int		moveStart_;		//! 動き始め
	int		moveEnd_;		//! 動き終わり

	float	move_;		//! 1フレームで動く距離

	bool	next_;		//! 次の攻撃の入力
	VECTOR3 dir_;		//! 方向

private:
	bool debug_nextFrame_;	//!デバッグ中の次のフレーム
};

#endif // _ATTACK_BASE_STATE_H_
