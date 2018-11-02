//-----------------------------------------------------------------------------
//
//	バイナリファイルのInput[BinInput.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "BinInput.h"
#include "../GameSystems.h"
#include "Input.h"
#include <time.h>

BinInput::BinInput(string filename, bool read) : read_(read), filename_(filename), frameCnt_((read)? -1 : 0), end_(false)
{
	ZeroMemory(&data_, sizeof(BIN_EXCHANGE) * TIME_FRAME);
	if (read) { ReadBin(); }
}

// キーボードの終了処理
BinInput::~BinInput(void)
{
	ZeroMemory(data_, TIME_FRAME);
}

// キーボードの更新処理
void BinInput::Update(INPUT_STATE& state)
{
	if (!read_) { return; }
	BIN_EXCHANGE newBin = data_[++frameCnt_];
	INPUT_STATE newState;
	ZeroMemory(&newState, sizeof(INPUT_STATE));

	newState.button.press   = newBin.button;
	newState.axis.bit.press = newBin.axis;

	ButtonPress(newState.button, state.button);

	ButtonPress(newState.axis.bit, state.axis.bit);
}

void BinInput::GuiUpdate(void)
{
	ImGui::Text("time %d, frameCnt %d", frameCnt_ / 60, frameCnt_);
	ImGui::Text("STATE %d", sizeof(BinInput));
	ImGui::SameLine();
	ImGui::Text("DATA  %d", sizeof(BIN_EXCHANGE));

	ImGui::Text("press   ");
	GuiCheck(button_.press, axis_.press);

	ImGui::Text("trig    ");
	GuiCheck(button_.trigger, axis_.trigger);

	ImGui::Text("release ");
	GuiCheck(button_.release, axis_.release);

	ImGui::Text("repeat  ");
	GuiCheck(button_.repeat, axis_.repeat);
	ImGui::Text("BuCt %3d", button_.repeatCnt);
	ImGui::SameLine();
	ImGui::Text("AxCt %3d", axis_.repeatCnt);
}

void BinInput::GuiCheck(const WORD& button, const WORD& axis)
{
	if (button & Input::GAMEPAD_SQUARE)
	{
		ImGui::SameLine();
		ImGui::Text("square ");
	}
	if (button & Input::GAMEPAD_CROSS)
	{
		ImGui::SameLine();
		ImGui::Text("cross ");
	}
	if (button & Input::GAMEPAD_CIRCLE)
	{
		ImGui::SameLine();
		ImGui::Text("circle ");
	}
	if (button & Input::GAMEPAD_TRIANGLE)
	{
		ImGui::SameLine();
		ImGui::Text("triangle ");
	}

	if (button & Input::GAMEPAD_L1)
	{
		ImGui::SameLine();
		ImGui::Text("L1 ");
	}
	if (button & Input::GAMEPAD_R1)
	{
		ImGui::SameLine();
		ImGui::Text("R1 ");
	}
	if (button & Input::GAMEPAD_L2)
	{
		ImGui::SameLine();
		ImGui::Text("L2 ");
	}
	if (button & Input::GAMEPAD_R2)
	{
		ImGui::SameLine();
		ImGui::Text("R2 ");
	}

	if (button & Input::GAMEPAD_SELECT)
	{
		ImGui::SameLine();
		ImGui::Text("OPTIONS ");
	}

	if (button & Input::GAMEPAD_L3)
	{
		ImGui::SameLine();
		ImGui::Text("L3 ");
	}
	if (button & Input::GAMEPAD_R3)
	{
		ImGui::SameLine();
		ImGui::Text("R3 ");
	}

	if (button & Input::GAMEPAD_START)
	{
		ImGui::SameLine();
		ImGui::Text("TOUCH ");
	}
	if (button & Input::GAMEPAD_UP)
	{
		ImGui::SameLine();
		ImGui::Text("up");
	}
	if (button & Input::GAMEPAD_RIGHT)
	{
		ImGui::SameLine();
		ImGui::Text("rg");
	}
	if (button & Input::GAMEPAD_DOWN)
	{
		ImGui::SameLine();
		ImGui::Text("dw");
	}
	if (button & Input::GAMEPAD_LEFT)
	{
		ImGui::SameLine();
		ImGui::Text("lf");
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

}

int BinInput::AxisJudge(WORD& gamepad, WORD& axis)
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

void BinInput::ReadBin(void)
{
	FILE* pFile;

	fopen_s(&pFile, filename_.c_str(), "r");
	if (pFile)
	{
		fread(data_, sizeof(BIN_EXCHANGE), TIME_FRAME, pFile);
		// ファイルを閉じる
		fclose(pFile);
	}
}

void BinInput::WriteBin(WORD button, WORD axis)
{
	if (end_) { return; }
	if (frameCnt_ < 0 || read_) { return; }
	else if (frameCnt_ == TIME_FRAME)
	{
		WriteUninit();
		end_ = true;
	}

	data_[frameCnt_] = { button, axis };
	frameCnt_++;
}

void BinInput::WriteUninit(void)
{
	FILE* pFile;

	// ファイルオープン
	fopen_s(&pFile, filename_.c_str(), "a");
	if (pFile)
	{
		fwrite(data_, sizeof(BIN_EXCHANGE), TIME_FRAME, pFile);
		// ファイルを閉じる
		fclose(pFile);
	}

	read_     = true;
	frameCnt_ = -1;
}
