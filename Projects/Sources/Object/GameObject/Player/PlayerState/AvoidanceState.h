/*
 * @file		AvoidanceState.h
 * @brief		ņð
 * @author		ËāVãÄū
 * @data		2018/11/10
 */
#ifndef _AVOIDANCE_STATE_H_
#define _AVOIDANCE_STATE_H_

#include "PlayerState.h"

class AvoidanceState : public PlayerState
{
public:
	//! @def	gpÉÁï·éX^~i
	static constexpr int DEC_STAMINA = 30;

	AvoidanceState(void);
	~AvoidanceState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;

private:
	VECTOR3 dir_;		//! ûü
};

#endif // _AVOIDANCE_STATE_H_
