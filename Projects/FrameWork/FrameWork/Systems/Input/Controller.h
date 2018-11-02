//-----------------------------------------------------------------------------
//
//	仮想コントローラ[Controller.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "Input.h"
#include "../DebugSetting/GUI.h"

//-----------------------------------------------------------------------------
// クラス宣言
//-----------------------------------------------------------------------------
class BaseInput;
class Controller : public GUI
{
	friend class Input;
public:
	enum CtrlNum
	{
		X = 0,
		PS4,
		Bin,
		Key,
		NOT_CONECT
	};
	~Controller(void);

	Input*               GetInput(void)   { return input_;    }
	BYTE                 GetNumber(void)  { return number_;   }
	BYTE                 GetCtrlNum(void) { return ctrlNum_;  }
	LPDIRECTINPUTDEVICE8 GetDevCtrl(void) { return pDevCtrl_; }

	INPUT_BUTTON GetButton(void) { return state_.button; }
	INPUT_AXIS   GetAxis(void)   { return state_.axis;   }
	int          Press  (WORD gamepad, int key = 0, bool minus = false);
	int          Trigger(WORD gamepad, int key = 0, bool minus = false);
	int          Release(WORD gamepad, int key = 0, bool minus = false);
	int          Repeat (WORD gamepad, int key = 0, bool minus = false);

	int			 PressRange(WORD gamepad, int minus_key, int plus_key);

	void GuiUpdate(void) override;

	void Cutting(void);

private:
	Controller(Input* input, BYTE number);
	void Update(void);

	LPDIRECTINPUT8 GetInputDevice(void) { return this->pInput_; };
	bool ConnectPS4Ctrl(void);
	bool ConnectXCtrl(void);
	WORD SelectKey(WORD gamepad);
	int  ButtonAxisJudge(WORD& gamepad, WORD& button, WORD& axis);
	int  AxisJudge(WORD& gamepad, WORD& axis);

	INPUT_STATE state_;

	BaseInput* ctrl_;

	GUID inst_;
	GUID temp_;

	Input* input_;
	BYTE   number_;
	BYTE   ctrlNum_;

	LPDIRECTINPUT8       pInput_;		// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 pDevCtrl_;		// 入力デバイス(キーボード)へのポインタ

	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID* pContext);
};

#endif