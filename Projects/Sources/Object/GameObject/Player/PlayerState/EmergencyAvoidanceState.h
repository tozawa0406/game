/*
 * @file		EmergencyAvoidanceState.h
 * @brief		Ų}ņð
 * @author		ËāVãÄū
 * @data		2018/11/10
 */
#ifndef _EMERGENCY_AVOIDANCE_STATE_H_
#define _EMERGENCY_AVOIDANCE_STATE_H_

#include "PlayerState.h"

class EmergencyAvoidanceState : public PlayerState
{
public:
	//! @def	gpÉÁï·éX^~i
	static constexpr int DEC_STAMINA = 30;

	EmergencyAvoidanceState(void);
	~EmergencyAvoidanceState(void);

	void Init(Player* player, Controller* ctrl) override;
	void Uninit(void) override;
	PlayerState* Update(void) override;

	void GuiUpdate(void) override;

private:
	VECTOR3 dir_;		//! ûü
	int		cnt_;
};

#endif // _EMERGENCY_AVOIDANCE_STATE_H_
