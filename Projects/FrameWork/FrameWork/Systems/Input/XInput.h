//-----------------------------------------------------------------------------
//
//	XInput[XInput.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _XINPUT__H_
#define _XINPUT__H_

#include "Input.h"

#include "BaseInput.h"
#include "Controller.h"
#include "../DebugSetting/GUI.h"

class XInput : public BaseInput, public GUI
{
	friend Input;
public:
	static constexpr float THUMB_MAX = 32768.0f;
	static constexpr float TRIGGER_MAX = 255.0f;
	static constexpr float NOT_ENABLE_THUMB   = (THUMB_MAX   / (float)Input::RANGE / 2.f);
	static constexpr float NOT_ENABLE_TRIGGER = (TRIGGER_MAX / (float)Input::RANGE / 2.f);

	static constexpr int AXIS_LX   = 0xa000;
	static constexpr int AXIS_LY   = 0xb000;
	static constexpr int AXIS_RX   = 0xc000;
	static constexpr int AXIS_RY   = 0xd000;

	static constexpr int TRIGGER_L = 0xe000;
	static constexpr int TRIGGER_R = 0xf000;

	~XInput(void);
	void GuiUpdate(void) override;

private:
	XInput(Controller* ctrl);						// コンストラクタ
	void Update(INPUT_STATE& state) override;		// 実際にコントローラを取得する処理

	void CreateButton(INPUT_BUTTON& newState, XINPUT_STATE& xs);
	void CreateAxis  (INPUT_AXIS&   newState, XINPUT_STATE& xs, INPUT_AXIS& state);

	void GuiCheck(const WORD& button, const SHORT& axis, const byte& trigger);

	static constexpr float rangeTrig  = 1 / (TRIGGER_MAX / (float)Input::RANGE);
	static constexpr float rangeThumb = 1 / (THUMB_MAX   / (float)Input::RANGE);

	DWORD       dwPacketNumber_;
	Controller* ctrl_;
};

#endif // _XINPUT__H_
