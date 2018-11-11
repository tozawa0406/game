/*
 * @file		DrawnMoveState.h
 * @brief		������Ԃ̈ړ�����
 * @author		���V�đ�
 * @data		2018/11/10
 */
#ifndef _DRAWN_MOVE_STATE_H_
#define _DRAWN_MOVE_STATE_H_

#include "../PlayerState.h"

class DrawnMoveState : public PlayerState
{
public:
	DrawnMoveState(void);
	~DrawnMoveState(void);

	void Init(PlayerHunter* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

private:
	VECTOR2 inputDir_;
};

#endif // _DRAWN_MOVE_STATE_H_
