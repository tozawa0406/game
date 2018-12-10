//-----------------------------------------------------------------------------
//
//	入力管理[Input.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_

#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)		//dinput.hを使う場合は前述しておく
#include <dinput.h>

#include "../../Define/Define.h"
#include "../BaseManager.h"

struct INPUT_BUTTON
{
	WORD press;
	WORD trigger;
	WORD release;
	WORD repeat;
	WORD repeatCnt;
};

struct INPUT_AXIS
{
	SHORT stickLX;
	SHORT stickLY;
	SHORT stickRX;
	SHORT stickRY;

	SHORT triggerL;
	SHORT triggerR;

	INPUT_BUTTON bit;
};

struct INPUT_STATE
{
	INPUT_BUTTON button;
	INPUT_AXIS   axis;
};

//-----------------------------------------------------------------------------
// クラス宣言
//-----------------------------------------------------------------------------
class Controller;
class KeyInput;
class Input : public Interface
{
public:
	static constexpr int MAX_CONTROLLERS = 2;				//扱うコントローラの最大数

	// アナログスティックの傾きの最大値
	static constexpr int RANGE = 100;		// 0～いくつまでか
	static constexpr int REPEAT_RANGE = 30;

	static constexpr int GAMEPAD_SQUARE   = 0x0001;
	static constexpr int GAMEPAD_CROSS    = 0x0002;
	static constexpr int GAMEPAD_CIRCLE   = 0x0004;
	static constexpr int GAMEPAD_TRIANGLE = 0x0008;

	static constexpr int GAMEPAD_L1       = 0x0010;
	static constexpr int GAMEPAD_R1       = 0x0020;
	static constexpr int GAMEPAD_L3       = 0x0040;
	static constexpr int GAMEPAD_R3       = 0x0080;

	static constexpr int GAMEPAD_START    = 0x0100;
	static constexpr int GAMEPAD_SELECT   = 0x0200;
	static constexpr int GAMEPAD_HOME     = 0x0400;
	static constexpr int GAMEPAD_SHARE    = 0x0800;

	static constexpr int GAMEPAD_UP       = 0x1000;
	static constexpr int GAMEPAD_RIGHT    = 0x2000;
	static constexpr int GAMEPAD_DOWN     = 0x4000;
	static constexpr int GAMEPAD_LEFT     = 0x8000;

	static constexpr int AXIS_LX    = 0xa000;
	static constexpr int AXIS_LY    = 0xb000;
	static constexpr int AXIS_RX    = 0xc000;
	static constexpr int AXIS_RY    = 0xd000;

	static constexpr int GAMEPAD_L2 = 0xe000;
	static constexpr int GAMEPAD_R2 = 0xf000;

	static constexpr int JUDGE_LX_PLUS  = 0x0001;
	static constexpr int JUDGE_LX_MINUS = 0x0002;
	static constexpr int JUDGE_LY_PLUS  = 0x0004;
	static constexpr int JUDGE_LY_MINUS = 0x0008;
	static constexpr int JUDGE_RX_PLUS  = 0x0010;
	static constexpr int JUDGE_RX_MINUS = 0x0020;
	static constexpr int JUDGE_RY_PLUS  = 0x0040;
	static constexpr int JUDGE_RY_MINUS = 0x0080;
	static constexpr int JUDGE_L2       = 0x0100;
	static constexpr int JUDGE_R2       = 0x0200;

	friend class Systems;

	Input(Systems* systems);
	~Input(void)           override;
	HRESULT     Init(void) override;
	KeyInput*   GetKeyboard(void)            { return keyboard_;                                }
	byte        NowNumber(void)              { return nowNumber_;                               }
	byte        CallBackController(void)     { return numCallBackController_;                   }
	Controller* GetCtrl(int i)               { return controller_[min(i, MAX_CONTROLLERS - 1)]; }
	void        CallBackControllerZero(void) { nowNumber_ = 0;                                  }
	void        CallBackControllerPlus(void) { nowNumber_++;                                    }

	void        UseBinInput(int i, string binFilename, bool read = true);

private:
	void Update(void) override;

	KeyInput*   keyboard_;
	Controller* controller_[MAX_CONTROLLERS];

	byte        nowNumber_;
	byte        numCallBackController_;
};

#endif