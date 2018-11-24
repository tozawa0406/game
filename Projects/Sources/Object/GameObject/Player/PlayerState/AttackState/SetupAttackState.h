/*
 * @file		SetupState.h
 * @brief		�[��������
 * @author		���V�đ�
 * @data		2018/11/11
 */
#ifndef _SETUP_STATE_H_
#define _SETUP_STATE_H_

#include "PlayerState.h"

class SetupState : public PlayerState
{
public:
	SetupState(void);
	~SetupState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;

private:
	bool isDraw_;	//! �����t���O
	bool isAttack_;
};

#endif // _SETUP_STATE_H_
