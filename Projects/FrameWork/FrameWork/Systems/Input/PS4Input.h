//-----------------------------------------------------------------------------
//
//	PS4(Direct)ÇÃInput[PS4Input.h]
//	Auther : åÀ‡V„ƒëæ
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _PS4INPUT_H_
#define _PS4INPUT_H_

#include "Input.h"
#include <functional>

#include "BaseInput.h"
#include "Controller.h"

enum PS4InputNum
{
	SQUARE   = 0,
	CROSS    = 1,
	CIRCLE   = 2,
	TRIANGLE = 3,
	L1       = 4,
	R1       = 5,
	L2       = 6,
	R2       = 7,
	SHARE    = 8,
	OPTIONS  = 9,
	L3       = 10,
	R3       = 11,
	PSBUTTON = 12,
	TOUCHPAD = 13
};

//-----------------------------------------------------------------------------
// ÉNÉâÉXêÈåæ
//-----------------------------------------------------------------------------
class PS4Input : public BaseInput
{
	friend Input;

public:
	void GuiUpdate(void) override;

private:
	PS4Input(Controller* ctrl);
	~PS4Input(void);
	void Update(INPUT_STATE& state) override;

	void CreateButton(INPUT_BUTTON& newState, DIJOYSTATE& js);
	void CreateAxis(INPUT_AXIS& newState, DIJOYSTATE& js, INPUT_AXIS& state);

	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext);

	Controller* ctrl_;
};

#endif