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
#include <BluetoothAPIs.h>
#pragma comment (lib, "Bthprops")

Input::Input(Systems* systems) : Interface(systems), nowNumber_((byte)-1), numCallBackController_(0)
{
}

// キーボードの初期化
HRESULT Input::Init(void)
{
/*
	{
		//Device の検索
		BLUETOOTH_DEVICE_SEARCH_PARAMS	searchparam;
		memset(&searchparam, 0, sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS));
		searchparam.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
		searchparam.fReturnAuthenticated = TRUE;
		searchparam.fReturnRemembered = TRUE;
		searchparam.fReturnConnected = TRUE;
		searchparam.fReturnUnknown = TRUE;
		searchparam.fIssueInquiry = FALSE;
		searchparam.cTimeoutMultiplier = 0;
		searchparam.hRadio = NULL; // all

		HBLUETOOTH_DEVICE_FIND	hfind = NULL;
		BLUETOOTH_DEVICE_INFO	devinfo;
		memset(&devinfo, 0, sizeof(BLUETOOTH_DEVICE_INFO));
		devinfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
		if (hfind = BluetoothFindFirstDevice(&searchparam, &devinfo)) {
			do {
				devinfo = devinfo;
				// bluetoothの接続解除
//				BluetoothRemoveDevice(&devinfo.Address);
				// bluetoothの接続
//				BluetoothSetServiceState(hradio, &devinfo, &HumanInterfaceDeviceServiceClass_UUID, BLUETOOTH_SERVICE_ENABLE);

			} while (BluetoothFindNextDevice(hfind, &devinfo));
			BluetoothFindDeviceClose(hfind);
		}
	}
*/

	keyboard_ = new KeyInput(this);

	if (FAILED(keyboard_->Init())) { return E_FAIL; }

	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		controller_[i] = nullptr;
	}

	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		nowNumber_ = (byte)i;
		controller_[i] = new Controller(this, (byte)i);

		if (controller_[i]->ConnectXCtrl())
		{
			controller_[i]->ctrl_ = new XInput(controller_[i]);
		}
		else if (controller_[i]->ConnectPS4Ctrl())
		{
			controller_[i]->ctrl_ = new PS4Input(controller_[i]);
		}
	}
	nowNumber_ = (byte)-1;

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
		nowNumber_ = (byte)i;

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
	nowNumber_ = (byte)-1;

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
