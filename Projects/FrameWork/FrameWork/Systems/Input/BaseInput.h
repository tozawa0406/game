//-----------------------------------------------------------------------------
//
//	コントローラ基底[BaseInput.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _BASE_INPUT_H_
#define _BASE_INPUT_H_

#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)		//dinput.hを使う場合は前述しておく
#include <dinput.h>

#include "../../Define/Define.h"
#include "Input.h"

//-----------------------------------------------------------------------------
// クラス宣言
//-----------------------------------------------------------------------------
class BaseInput
{
	friend class Controller;
public:
	virtual ~BaseInput(void) {}

protected:
	BaseInput(void) {}

	void ButtonPress(INPUT_BUTTON& newState, INPUT_BUTTON& button)
	{
		button.trigger = (button.press ^ newState.press) & newState.press;
		button.release = (button.press ^ newState.press) & button.press;

		this->RepeatCheck(newState.press, button, Input::REPEAT_RANGE);

		button.press = newState.press;
	}
	void RepeatCheck(WORD& newStatePress, INPUT_BUTTON& button, int range)
	{
		if (newStatePress)
		{
			if (button.repeatCnt < range)
			{
				button.repeatCnt++;
				if (button.repeatCnt == 1)
				{
					button.repeat = newStatePress;
				}
				else
				{
					button.repeat = 0;
				}
			}
			else
			{
				button.repeatCnt = 0;
			}
		}
		else
		{
			button.repeatCnt = 0;
			button.repeat = 0;
		}
	}
	void AxisPress(WORD& axisPress, const SHORT& newAxis, SHORT& axis, WORD bit, bool inverted)
	{
		if (inverted) { bit *= 2; }
		if (newAxis > Half(Input::RANGE))
		{
			axisPress |= bit;
		}
		else if (newAxis < -Half(Input::RANGE))
		{
			if (inverted) { bit /= 4; }
			axisPress |= bit * 2;
		}
		axis = newAxis;
	}

	virtual void Update(INPUT_STATE& state) = 0;

	virtual void GuiUpdate(void) {};
};

#endif