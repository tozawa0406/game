//-----------------------------------------------------------------------------
//
//	PS4(Direct)��Input[PS4Input.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "PS4Input.h"
#include "../../Windows/Windows.h"
#include "../GameSystems.h"

BOOL CALLBACK PS4Input::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	UNREFERENCED_PARAMETER(pContext);

	HRESULT     hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - Input::RANGE;
	diprg.lMax = 0 + Input::RANGE;
	hr = Systems::Instance()->GetInput()->GetCtrl(Systems::Instance()->GetInput()->NowNumber())->GetDevCtrl()->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) { return DIENUM_STOP; }

	return DIENUM_CONTINUE;
}

PS4Input::PS4Input(Controller* ctrl) : ctrl_(ctrl)
{
	Windows* window = ctrl->GetInput()->GetSystems()->GetWindow();
	//���Ŏg���ĂȂ��ϐ��̃��[�j���O��������
	UNREFERENCED_PARAMETER(window->GetHWND());
	if (ctrl->GetDevCtrl() == nullptr) { return; }

	// �f�[�^�t�H�[�}�b�g��ݒ�
	HRESULT hr = ctrl->GetDevCtrl()->SetDataFormat(&c_dfDIJoystick);
	if (window->ErrorMessage("�R���g���[���̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�G���[", hr))
	{ return; }

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = ctrl->GetDevCtrl()->SetCooperativeLevel(window->GetHWND(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (window->ErrorMessage("�R���g���[���̋������[�h��ݒ�ł��܂���ł����B", "�G���[", hr))
	{ return; }

	// �\���L�[�͈͎̔w��
	hr = ctrl->GetDevCtrl()->EnumObjects(EnumAxesCallback, (VOID*)window->GetHWND(), DIDFT_AXIS);
	if (window->ErrorMessage("�R���g���[���͈̔͂��w��ł��܂���ł����B", "�G���[", hr))
	{ return; }

	// �A�N�Z�X�����l��(���͐���J�n)
	if (FAILED(ctrl->GetDevCtrl()->Poll()))
	{
		hr = ctrl->GetDevCtrl()->Acquire();
		while (hr == DIERR_INPUTLOST)
		{
			hr = ctrl->GetDevCtrl()->Acquire();
		}
	}
}

// �L�[�{�[�h�̏I������
PS4Input::~PS4Input(void)
{
}

// �L�[�{�[�h�̍X�V����
void PS4Input::Update(INPUT_STATE& state)
{
	if (ctrl_->GetDevCtrl() == nullptr) { return; }

	DIJOYSTATE js;
	if (SUCCEEDED(ctrl_->GetDevCtrl()->Poll()))
	{
		if (SUCCEEDED(ctrl_->GetDevCtrl()->GetDeviceState(sizeof(DIJOYSTATE), &js)))
		{
			INPUT_STATE newState;
			ZeroMemory(&newState, sizeof(INPUT_STATE));

			CreateButton(newState.button, js);
			ButtonPress(newState.button, state.button);

			CreateAxis(newState.axis, js, state.axis);
			ButtonPress(newState.axis.bit, state.axis.bit);
		}
	}
	else
	{
		ctrl_->Cutting();
	}
}

void PS4Input::CreateButton(INPUT_BUTTON& newState, DIJOYSTATE& js)
{
	int temp = Input::GAMEPAD_SQUARE;
	for (int i = 0; i < 4; i++)
	{
		if (js.rgbButtons[PS4InputNum::SQUARE + i])
		{
			newState.press |= temp;
		}
		temp *= 2;
	}

	temp = Input::GAMEPAD_L1;
	for (int i = 0; i < 2; i++)
	{
		if (js.rgbButtons[PS4InputNum::L1 + i])
		{
			newState.press |= temp;
		}
		temp *= 2;
	}

	temp = Input::GAMEPAD_L3;
	for (int i = 0; i < 2; i++)
	{
		if (js.rgbButtons[PS4InputNum::L3 + i])
		{
			newState.press |= temp;
		}
		temp *= 2;
	}

	if (js.rgbButtons[PS4InputNum::OPTIONS])
	{
		newState.press |= Input::GAMEPAD_SELECT;
	}
	if (js.rgbButtons[PS4InputNum::TOUCHPAD])
	{
		newState.press |= Input::GAMEPAD_START;
	}

	if (js.rgbButtons[PS4InputNum::PSBUTTON])
	{
		newState.press |= Input::GAMEPAD_HOME;
	}
	if (js.rgbButtons[PS4InputNum::SHARE])
	{
		newState.press |= Input::GAMEPAD_SHARE;
	}


	temp = Input::GAMEPAD_UP;
	for (int i = 0; i < 4; i++)
	{
		if (js.rgdwPOV[0] == (unsigned long)(9000 * i))
		{
			newState.press |= temp;
		}
		temp *= 2;
	}
}

void PS4Input::CreateAxis(INPUT_AXIS& newState, DIJOYSTATE& js, INPUT_AXIS& state)
{
	newState.triggerL = (SHORT)max(js.lRx, 0);
	newState.triggerR = (SHORT)max(js.lRy, 0);

	newState.stickLX = (SHORT)js.lX;
	newState.stickLY = (SHORT)js.lY;
	newState.stickRX = (SHORT)js.lZ;
	newState.stickRY = (SHORT)js.lRz;

	AxisPress(newState.bit.press, newState.stickLX, state.stickLX, Input::JUDGE_LX_PLUS, false);
	AxisPress(newState.bit.press, newState.stickLY, state.stickLY, Input::JUDGE_LY_PLUS, true);
	AxisPress(newState.bit.press, newState.stickRX, state.stickRX, Input::JUDGE_RX_PLUS, false);
	AxisPress(newState.bit.press, newState.stickRY, state.stickRY, Input::JUDGE_RY_PLUS, true);
	AxisPress(newState.bit.press, newState.triggerL, state.triggerL, Input::JUDGE_L2, false);
	AxisPress(newState.bit.press, newState.triggerR, state.triggerR, Input::JUDGE_R2, false);
}

void PS4Input::GuiUpdate(void)
{
}
