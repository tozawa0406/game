/*
 * @file		PaidWaitState.h
 * @brief		�[���̑ҋ@����
 * @author		���V�đ�
 * @data		2018/11/10
 */
#ifndef _PAID_WAIT_STATE_H_
#define _PAID_WAIT_STATE_H_

#include "../PlayerState.h"

class PaidWaitState : public PlayerState
{
public:
	PaidWaitState(void);
	~PaidWaitState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;

private:
	int waitCnt_;		//! �҂�����
};

#endif // _PAID_WAIT_STATE_H_
