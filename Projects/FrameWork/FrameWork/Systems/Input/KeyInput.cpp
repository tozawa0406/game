//-----------------------------------------------------------------------------
//
//	キーボードInput[KeyInput.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "KeyInput.h"
#include "../../Windows/Windows.h"
#include "../GameSystems.h"
#include "Input.h"

KeyInput::KeyInput(Input* input) : input_(input), pInput_(nullptr), pDevKeyboard_(nullptr)
{
}

// キーボードの初期化
HRESULT KeyInput::Init(void)
{
	Windows* window = input_->GetSystems()->GetWindow();

	HRESULT hr;
	//↓で使ってない変数のワーニングを消せる
	UNREFERENCED_PARAMETER(window->GetHWND());

	// 入力処理の初期化
	if (pInput_ == nullptr)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(window->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pInput_, NULL);
		if (window->ErrorMessage("DirectInputオブジェクトが作れません", "エラー", hr))
		{
			return E_FAIL;
		}
	}
	
	// デバイスの作成
	hr = pInput_->CreateDevice(GUID_SysKeyboard, &pDevKeyboard_, NULL);
	if(window->ErrorMessage("キーボードがありません", "エラー", hr))
	{ return E_FAIL; }

	// データフォーマットを設定
	hr = pDevKeyboard_->SetDataFormat(&c_dfDIKeyboard);
	if(window->ErrorMessage("キーボードのデータフォーマットを設定できませんでした。", "エラー", hr))
	{ return E_FAIL; }

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = pDevKeyboard_->SetCooperativeLevel(window->GetHWND(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(window->ErrorMessage("キーボードの協調モードを設定できませんでした。", "エラー", hr))
	{ return E_FAIL; }

	// キーボードへのアクセス権を獲得(入力制御開始)
	pDevKeyboard_->Acquire();

	ZeroMemory(state_, sizeof(INPUT_BUTTON) * KEY_MAX);

	return S_OK;
}

// キーボードの終了処理
KeyInput::~KeyInput(void)
{
	if(pDevKeyboard_ != nullptr)
	{// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		pDevKeyboard_->Unacquire();

		pDevKeyboard_->Release();
		pDevKeyboard_ = nullptr;
	}
}

// キーボードの更新処理
void KeyInput::Update(void)
{
	byte newKey[KEY_MAX];

	// デバイスからデータを取得
	if(SUCCEEDED(pDevKeyboard_->GetDeviceState(sizeof(newKey), newKey)))
	{
		for(int keyCnt = 0; keyCnt < KEY_MAX; keyCnt++)
		{
			// キートリガー・リリース情報を生成
			state_[keyCnt].trigger = (state_[keyCnt].press ^ newKey[keyCnt]) & newKey[keyCnt];
			//											(過去^現在) & 現在
			//									     1 =  0    1       1
			//										 0 =  1    1       1
			//										 0 =  1    0       0
			state_[keyCnt].release = (state_[keyCnt].press ^ newKey[keyCnt]) & state_[keyCnt].press;
			//										(過去^現在) & 過去
			//										 1 =  1    0       1
			//										 0 =  0    1       0

			// キーリピート情報を生成
			if(newKey[keyCnt])
			{
				if(state_[keyCnt].repeatCnt < Input::REPEAT_RANGE)
				{
					state_[keyCnt].repeatCnt++;
					if (state_[keyCnt].repeatCnt == 1)
					{
						state_[keyCnt].repeat = newKey[keyCnt];
					}
					else
					{
						state_[keyCnt].repeat = 0;
					}
				}
				else
				{
					state_[keyCnt].repeatCnt = 0;
				}
			}
			else
			{
				state_[keyCnt].repeatCnt = 0;
				state_[keyCnt].repeat    = 0;
			}

			// キープレス情報を保存
			state_[keyCnt].press = newKey[keyCnt];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		pDevKeyboard_->Acquire();
	}
}

// キーボードのプレス状態を取得
bool KeyInput::Press(int key)
{
	return (state_[key].press & 0x80) ? true : false;
}

// キーボードのトリガー状態を取得
bool KeyInput::Trig(int key)
{
	return (state_[key].trigger & 0x80) ? true : false;
}

// キーボードのリリ－ス状態を取得
bool KeyInput::Release(int key)
{
	return (state_[key].release & 0x80) ? true : false;
}

// キーボードのリピート状態を取得
bool KeyInput::Repeat(int key)
{
	return (state_[key].repeat & 0x80) ? true : false;
}
