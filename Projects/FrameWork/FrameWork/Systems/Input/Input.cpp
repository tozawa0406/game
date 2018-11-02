//-----------------------------------------------------------------------------
//
//	入力管理[Input.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Input.h"

#include "Controller.h"
#include "BaseInput.h"
#include "XInput.h"
#include "PS4Input.h"
#include "KeyInput.h"
#include "BinInput.h"

Input::Input(Systems* systems) : Interface(systems), nowNumber_((BYTE)-1), numCallBackController_(0)
{
}

// キーボードの初期化
HRESULT Input::Init(void)
{
	keyboard_ = new KeyInput(this);

	if (FAILED(keyboard_->Init())) { return E_FAIL; }

	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		controller_[i] = nullptr;
	}

	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		nowNumber_ = (BYTE)i;
		controller_[i] = new Controller(this, (BYTE)i);

		if (controller_[i]->ConnectXCtrl())
		{
			controller_[i]->ctrl_ = new XInput(controller_[i]);
		}
		else if (controller_[i]->ConnectPS4Ctrl())
		{
			controller_[i]->ctrl_ = new PS4Input(controller_[i]);
		}
	}
	nowNumber_ = (BYTE)-1;

	return S_OK;
}

// キーボードの終了処理
Input::~Input(void)
{
	for (int i = 0; i < MAX_CONTROLLERS; i++) { DeletePtr(controller_[i]); }

	DeletePtr(keyboard_);
}

// キーボードの更新処理
void Input::Update(void)
{
	int nonConnect = 0;
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		nowNumber_ = (BYTE)i;

		if (keyboard_->Press(DIK_C))
		{
			if (controller_[i]->ConnectXCtrl())
			{
				DeletePtr(controller_[i]->ctrl_);
				controller_[i]->ctrl_ = new XInput(controller_[i]);
			}
			else if (controller_[i]->ConnectPS4Ctrl())
			{
				DeletePtr(controller_[i]->ctrl_);
				controller_[i]->ctrl_ = new PS4Input(controller_[i]);
			}
		}

		if (controller_[i]->ctrl_ == nullptr)
		{
			nonConnect++;
		}
		controller_[i]->Update();
	}
	nowNumber_ = (BYTE)-1;

	if (nonConnect > 0)
	{
		keyboard_->Update();
	}
}

void Input::UseBinInput(int i, std::string filename, bool read)
{
	controller_[i]->ctrlNum_ = Controller::Bin;
	DeletePtr(controller_[i]->ctrl_);
	controller_[i]->ctrl_ = new BinInput(filename, read);
}
