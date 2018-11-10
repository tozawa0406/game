/*
 * @file		DrawnWait.h
 * @brief		�����̑ҋ@����
 * @author		���V�đ�
 * @data		2018/11/10
 */
#ifndef _DRAWN_WAIT_ACTION_H_
#define _DRAWN_WAIT_ACTION_H_

#include "../PlayerAction.h"

class DrawnWaitAction : public PlayerAction
{
public:
	DrawnWaitAction(void);
	~DrawnWaitAction(void);

	void Init(PlayerHunter* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerAction* Update(void) override;

};

#endif // _DRAWN_MOVE_ACTION_H_
