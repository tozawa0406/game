//-----------------------------------------------------------------------------
//
//	仮想コントローラ[Controller.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Controller.h"
#include "Input.h"
#include "BaseInput.h"
#include "../GameSystems.h"
#include "../../Windows/Windows.h"
#include "KeyInput.h"
#include "XInput.h"
#include "BinInput.h"

Controller::Controller(Input* input, byte number) : GUI(input->GetSystems(), nullptr, "ctrl"),
                                                    input_(input), number_(number), ctrl_(nullptr), pInput_(nullptr), pDevCtrl_(nullptr)
{
	ZeroMemory(&state_, sizeof(INPUT_STATE));
	if (input->GetKeyboard() != nullptr) { ctrlNum_ = Key;        }
	else                                 { ctrlNum_ = NOT_CONECT; }
	// 入力処理の初期化
	if (pInput_ == nullptr)
	{
		// DirectInputオブジェクトの作成
		HRESULT hr = DirectInput8Create(input->GetSystems()->GetWindow()->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pInput_, NULL);
		if (input->GetSystems()->GetWindow()->ErrorMessage("DirectInputオブジェクトが作れません", "エラー", hr))
		{ return; }
	}
}

Controller::~Controller(void)
{
	DeletePtr(ctrl_);
	if (pDevCtrl_)
	{// 入力デバイス(キーボード)の開放
	 // キーボードへのアクセス権を開放(入力制御終了)
		pDevCtrl_->Unacquire();

		pDevCtrl_->Release();
		pDevCtrl_ = nullptr;
	}
	// 入力処理の終了処理
	ReleasePtr(pInput_);
}

BOOL CALLBACK Controller::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID* pContext)
{
	UNREFERENCED_PARAMETER(pContext);
	Input* input = Systems::Instance()->GetInput();
	byte nowNumber = input->NowNumber();
	HRESULT hr = input->GetCtrl(nowNumber)->GetInputDevice()->CreateDevice(pdidInstance->guidInstance, &input->GetCtrl(nowNumber)->pDevCtrl_, NULL);

	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	for (int i = 0; i < Input::MAX_CONTROLLERS; i++)
	{
		if (i != nowNumber)
		{
			if (input->GetCtrl(i) != nullptr &&
				input->GetCtrl(i)->ctrl_ != nullptr)
			{
				GUID elseInst = input->GetCtrl(i)->inst_;
				if (elseInst.Data1 == pdidInstance->guidInstance.Data1 &&
					elseInst.Data2 == pdidInstance->guidInstance.Data2 &&
					elseInst.Data3 == pdidInstance->guidInstance.Data3)
				{
					return DIENUM_CONTINUE;
				}
			}
		}
	}

	input->GetCtrl(nowNumber)->inst_ = pdidInstance->guidInstance;
	input->GetCtrl(nowNumber)->temp_ = pdidInstance->guidProduct;

	return DIENUM_STOP;
}

void Controller::Update(void)
{
	if (ctrl_ != nullptr)
	{
		ctrl_->Update(state_);
	}
	
	switch (ctrlNum_)
	{
	case PS4:
		this->name = "PS4Input";
		break;
	case X:
		this->name = "XInput";
		break;
	case Key:
		this->name = "KeyInput";
		break;
	case Bin:
		this->name = "BinInput";
		break;
	case NOT_CONECT:
		this->name = "NotConect";
		break;
	}
}

void Controller::Cutting(void)
{
	if (input_->GetKeyboard() != nullptr) { ctrlNum_ = Key;        }
	else                                  { ctrlNum_ = NOT_CONECT; }
	DeletePtr(ctrl_);
	if (pDevCtrl_ != nullptr)
	{// 入力デバイス(キーボード)の開放
	 // キーボードへのアクセス権を開放(入力制御終了)
		pDevCtrl_->Unacquire();

		pDevCtrl_->Release();
		pDevCtrl_ = nullptr;
	}
}

bool Controller::ConnectPS4Ctrl(void)
{
	if (ctrlNum_ != Key && ctrlNum_ != NOT_CONECT && ctrlNum_ != Bin) { return false; }
	if (pDevCtrl_ == nullptr)
	{
		ZeroMemory(&inst_, sizeof(GUID));
		ZeroMemory(&temp_, sizeof(GUID));
		// デバイスの作成
		pInput_->EnumDevices(DI8DEVCLASS_GAMECTRL, Controller::EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
		if (pDevCtrl_ != nullptr)
		{
			ctrlNum_ = PS4;
			return true;
		}
	}
	return false;
}

bool Controller::ConnectXCtrl(void)
{
	if (ctrlNum_ != Key && ctrlNum_ != NOT_CONECT && ctrlNum_ != Bin) { return false; }

	XINPUT_STATE newState;
	if (XInputGetState(number_, &newState) == ERROR_SUCCESS)
	{
		ctrlNum_ = X;
		return true;
	}
	return false;
}

WORD Controller::SelectKey(WORD gamepad)
{
	if (ctrlNum_ == X)
	{
		switch (gamepad)
		{
		case Input::GAMEPAD_SQUARE:
			return XINPUT_GAMEPAD_X;
		case Input::GAMEPAD_CROSS:
			return XINPUT_GAMEPAD_A;
		case Input::GAMEPAD_CIRCLE:
			return XINPUT_GAMEPAD_B;
		case Input::GAMEPAD_TRIANGLE:
			return XINPUT_GAMEPAD_Y;
		case Input::GAMEPAD_L1:
			return XINPUT_GAMEPAD_LEFT_SHOULDER;
		case Input::GAMEPAD_R1:
			return XINPUT_GAMEPAD_RIGHT_SHOULDER;
		case Input::GAMEPAD_L2:
			return XInput::TRIGGER_L;
		case Input::GAMEPAD_R2:
			return XInput::TRIGGER_R;
		case Input::GAMEPAD_L3:
			return XINPUT_GAMEPAD_LEFT_THUMB;
		case Input::GAMEPAD_R3:
			return XINPUT_GAMEPAD_RIGHT_THUMB;
		case Input::GAMEPAD_START:
			return XINPUT_GAMEPAD_START;
		case Input::GAMEPAD_SELECT:
			return XINPUT_GAMEPAD_BACK;
		case Input::GAMEPAD_UP:
			return XINPUT_GAMEPAD_DPAD_UP;
		case Input::GAMEPAD_RIGHT:
			return XINPUT_GAMEPAD_DPAD_RIGHT;
		case Input::GAMEPAD_DOWN:
			return XINPUT_GAMEPAD_DPAD_DOWN;
		case Input::GAMEPAD_LEFT:
			return XINPUT_GAMEPAD_DPAD_LEFT;
		case Input::AXIS_LX:
			return XInput::AXIS_LX;
		case Input::AXIS_LY:
			return XInput::AXIS_LY;
		case Input::AXIS_RX:
			return XInput::AXIS_RX;
		case Input::AXIS_RY:
			return XInput::AXIS_RY;
		}
	}
	else if (ctrlNum_ == PS4 || ctrlNum_ == Bin)
	{
		return gamepad;
	}
	return 0;
}

int Controller::Press(WORD gamepad, int key, bool minus)
{
	if (ctrl_ == nullptr) 
	{
		if (ctrlNum_ == Key)
		{
			int plus = 1;
			if (minus) { plus *= -1; }
			return input_->GetKeyboard()->Press(key) * plus;
		}
		return 0; 
	}

	return ButtonAxisJudge(gamepad, state_.button.press, state_.axis.bit.press);
}

int Controller::Trigger(WORD gamepad, int key, bool minus)
{
	if (ctrl_ == nullptr)
	{
		if (ctrlNum_ == Key)
		{
			int plus = 1;
			if (minus) { plus *= -1; }
			return input_->GetKeyboard()->Trig(key) * plus;
		}
		return 0; 
	}

	return ButtonAxisJudge(gamepad, state_.button.trigger, state_.axis.bit.trigger);
}

int Controller::Release(WORD gamepad, int key, bool minus)
{
	if (ctrl_ == nullptr)
	{
		if (ctrlNum_ == Key)
		{
			int plus = 1;
			if (minus) { plus *= -1; }
			return input_->GetKeyboard()->Release(key) * plus;
		}
		return 0; 
	}

	return ButtonAxisJudge(gamepad, state_.button.release, state_.axis.bit.release);
}

int Controller::Repeat(WORD gamepad, int key, bool minus)
{
	if (ctrl_ == nullptr) 
	{
		if (ctrlNum_ == Key)
		{
			int plus = 1;
			if (minus) { plus *= -1; }
			return input_->GetKeyboard()->Repeat(key) * plus;
		}
		return 0; 
	}

	return ButtonAxisJudge(gamepad, state_.button.repeat, state_.axis.bit.repeat);
}

int Controller::PressRange(WORD gamepad, int minus_key, int plus_key)
{
	int r = 0;
	r += Press(gamepad, minus_key, true);
	r += Press(gamepad, plus_key, false);
	return r;
}

int Controller::ButtonAxisJudge(WORD& gamepad, WORD& button, WORD& axis)
{
	if (gamepad < Input::AXIS_LX)
	{
		return button & gamepad;
	}
	else
	{
		return AxisJudge(gamepad, axis);
	}
}

int Controller::AxisJudge(WORD& gamepad, WORD& axis)
{
	switch (gamepad)
	{
	case Input::AXIS_LX:
		if      (axis & Input::JUDGE_LX_PLUS ) { return 1;  }
		else if (axis & Input::JUDGE_LX_MINUS) { return -1; }
		break;
	case Input::AXIS_LY:
		if      (axis & Input::JUDGE_LY_PLUS ) { return 1;  }
		else if (axis & Input::JUDGE_LY_MINUS) { return -1; }
		break;
	case Input::AXIS_RX:
		if      (axis & Input::JUDGE_RX_PLUS ) { return 1;  }
		else if (axis & Input::JUDGE_RX_MINUS) { return -1; }
		break;
	case Input::AXIS_RY:
		if      (axis & Input::JUDGE_RY_PLUS ) { return 1;  }
		else if (axis & Input::JUDGE_RY_MINUS) { return -1; }
		break;
	case Input::GAMEPAD_L2:
		if (axis & Input::JUDGE_L2) { return 1; }
		break;
	case Input::GAMEPAD_R2:
		if (axis & Input::JUDGE_R2) { return 1; }
		break;
	}
	return 0;

}

void Controller::GuiUpdate(void)
{
	ImGui::Text("device Data1 %lu", inst_.Data1);
	ImGui::Text("device Data2 %o", inst_.Data2);
	ImGui::Text("device Data3 %o", inst_.Data3);
	ImGui::Text("device Data4 %c", inst_.Data4);

	ImGui::Text("product Data1 %lu", temp_.Data1);
	ImGui::Text("product Data2 %o", temp_.Data2);
	ImGui::Text("product Data3 %o", temp_.Data3);
	ImGui::Text("product Data4 %c", temp_.Data4);
	if (ctrlNum_ != Key && ctrlNum_ != NOT_CONECT)
	{
		ctrl_->GuiUpdate();
	}
}
