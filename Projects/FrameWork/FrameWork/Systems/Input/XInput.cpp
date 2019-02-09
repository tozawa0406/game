//-----------------------------------------------------------------------------
//
//	XInput[XInput.cpp]
//	Auther : åÀ‡V„ƒëæ
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "XInput.h"
#include "KeyInput.h"

XInput::XInput(Controller* ctrl): GUI(ctrl->GetInput()->GetSystems(), nullptr, "Ctrl"), ctrl_(ctrl)
{
}

XInput::~XInput(void)
{
}

void XInput::Update(INPUT_STATE& state)
{
	//àÍéûïœêî
	XINPUT_STATE xs;
	ZeroMemory(&xs, sizeof(XINPUT_STATE));

	//ÉRÉìÉgÉçÅ[ÉâÇÃèÛë‘éÊìæ
	//ê⁄ë±
	if (XInputGetState(ctrl_->GetNumber(), &xs) == ERROR_SUCCESS)
	{
		INPUT_STATE newState;
		ZeroMemory(&newState, sizeof(INPUT_STATE));

		CreateButton(newState.button, xs);
		ButtonPress(newState.button, state.button);

		CreateAxis(newState.axis, xs, state.axis);
		ButtonPress(newState.axis.bit, state.axis.bit);

		dwPacketNumber_ = xs.dwPacketNumber;
	}
	//îÒê⁄ë±
	else
	{
		ctrl_->Cutting();
	}
}

void XInput::CreateButton(INPUT_BUTTON& newState, XINPUT_STATE& xs)
{
	WORD temp = xs.Gamepad.wButtons;
	if (temp & XINPUT_GAMEPAD_A) { newState.press |= Input::GAMEPAD_CROSS;    }
	if (temp & XINPUT_GAMEPAD_B) { newState.press |= Input::GAMEPAD_CIRCLE;   }
	if (temp & XINPUT_GAMEPAD_X) { newState.press |= Input::GAMEPAD_SQUARE;   }
	if (temp & XINPUT_GAMEPAD_Y) { newState.press |= Input::GAMEPAD_TRIANGLE; }

	if (temp & XINPUT_GAMEPAD_DPAD_UP   ) { newState.press |= Input::GAMEPAD_UP;    }
	if (temp & XINPUT_GAMEPAD_DPAD_DOWN ) { newState.press |= Input::GAMEPAD_DOWN;  }
	if (temp & XINPUT_GAMEPAD_DPAD_LEFT ) { newState.press |= Input::GAMEPAD_LEFT;  }
	if (temp & XINPUT_GAMEPAD_DPAD_RIGHT) { newState.press |= Input::GAMEPAD_RIGHT; }

	if (temp & XINPUT_GAMEPAD_LEFT_SHOULDER ) { newState.press |= Input::GAMEPAD_L1; }
	if (temp & XINPUT_GAMEPAD_RIGHT_SHOULDER) { newState.press |= Input::GAMEPAD_R1; }
	if (temp & XINPUT_GAMEPAD_LEFT_THUMB    ) { newState.press |= Input::GAMEPAD_L3; }
	if (temp & XINPUT_GAMEPAD_RIGHT_THUMB   ) { newState.press |= Input::GAMEPAD_R3; }

	if (temp & XINPUT_GAMEPAD_START) { newState.press |= Input::GAMEPAD_START;  }
	if (temp & XINPUT_GAMEPAD_BACK ) { newState.press |= Input::GAMEPAD_SELECT; }
}

void XInput::CreateAxis(INPUT_AXIS& newState, XINPUT_STATE& xs, INPUT_AXIS& state)
{
	// MAXílÇÃ 0.01î{ÇäÑÇÈÇ±Ç∆Ç≈ 0Å`100ÇÃîÕàÕÇ≈éÊìæÇ∑ÇÈ
	newState.stickLX = (SHORT)(xs.Gamepad.sThumbLX * rangeThumb);
	newState.stickLY = (SHORT)(xs.Gamepad.sThumbLY * rangeThumb);
	newState.stickRX = (SHORT)(xs.Gamepad.sThumbRX * rangeThumb);
	newState.stickRY = (SHORT)(xs.Gamepad.sThumbRY * rangeThumb);

	AxisPress(newState.bit.press, newState.stickLX, state.stickLX, Input::JUDGE_LX_PLUS, false);
	AxisPress(newState.bit.press, -newState.stickLY, state.stickLY, Input::JUDGE_LY_PLUS, true);
	AxisPress(newState.bit.press, newState.stickRX, state.stickRX, Input::JUDGE_RX_PLUS, false);
	AxisPress(newState.bit.press, -newState.stickRY, state.stickRY, Input::JUDGE_RY_PLUS, false);

	newState.triggerL = (SHORT)(xs.Gamepad.bLeftTrigger  * rangeTrig);
	newState.triggerR = (SHORT)(xs.Gamepad.bRightTrigger * rangeTrig);

	AxisPress(newState.bit.press, newState.triggerL, state.triggerL, Input::JUDGE_L2, false);
	AxisPress(newState.bit.press, newState.triggerR, state.triggerR, Input::JUDGE_R2, false);
}

void XInput::GuiUpdate(void)
{
}

void XInput::GuiCheck(const WORD& button, const SHORT& axis, const byte& trigger)
{
	if (button & XINPUT_GAMEPAD_A)
	{
		ImGui::SameLine();
		ImGui::Text("A");
	}
	if (button & XINPUT_GAMEPAD_B)
	{
		ImGui::SameLine();
		ImGui::Text("B");
	}
	if (button & XINPUT_GAMEPAD_X)
	{
		ImGui::SameLine();
		ImGui::Text("X");
	}
	if (button & XINPUT_GAMEPAD_Y)
	{
		ImGui::SameLine();
		ImGui::Text("Y");
	}
	if (button & XINPUT_GAMEPAD_BACK)
	{
		ImGui::SameLine();
		ImGui::Text("Back");
	}
	if (button & XINPUT_GAMEPAD_START)
	{
		ImGui::SameLine();
		ImGui::Text("Start");
	}
	if (button & XINPUT_GAMEPAD_DPAD_UP)
	{
		ImGui::SameLine();
		ImGui::Text("up");
	}
	if (button & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		ImGui::SameLine();
		ImGui::Text("dw");
	}
	if (button & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		ImGui::SameLine();
		ImGui::Text("lf");
	}
	if (button & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		ImGui::SameLine();
		ImGui::Text("rg");
	}
	if (button & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		ImGui::SameLine();
		ImGui::Text("L1");
	}
	if (button & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		ImGui::SameLine();
		ImGui::Text("R1");
	}
	if (button & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		ImGui::SameLine();
		ImGui::Text("L3");
	}
	if (button & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		ImGui::SameLine();
		ImGui::Text("R3");
	}

	if (axis & 0x01)
	{
		ImGui::SameLine();
		ImGui::Text("+LX");
	}
	if (axis & 0x02)
	{
		ImGui::SameLine();
		ImGui::Text("-LX");
	}
	if (axis & 0x04)
	{
		ImGui::SameLine();
		ImGui::Text("+LY");
	}
	if (axis & 0x08)
	{
		ImGui::SameLine();
		ImGui::Text("-LY");
	}
	if (axis & 0x10)
	{
		ImGui::SameLine();
		ImGui::Text("+RX");
	}
	if (axis & 0x20)
	{
		ImGui::SameLine();
		ImGui::Text("-RX");
	}
	if (axis & 0x40)
	{
		ImGui::SameLine();
		ImGui::Text("+RY");
	}
	if (axis & 0x80)
	{
		ImGui::SameLine();
		ImGui::Text("-RY");
	}

	if (trigger & 0x1)
	{
		ImGui::SameLine();
		ImGui::Text("LTR");
	}
	if (trigger & 0x2)
	{
		ImGui::SameLine();
		ImGui::Text("RTR");
	}
}
