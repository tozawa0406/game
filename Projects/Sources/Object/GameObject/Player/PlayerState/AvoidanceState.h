/*
 * @file		AvoidanceState.h
 * @brief		�������
 * @author		���V�đ�
 * @data		2018/11/10
 */
#ifndef _AVOIDANCE_STATE_H_
#define _AVOIDANCE_STATE_H_

#include "PlayerState.h"

class AvoidanceState : public PlayerState
{
public:
	AvoidanceState(void);
	~AvoidanceState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;

private:
	VECTOR3 dir_;		//! ����
};

#endif // _AVOIDANCE_STATE_H_
