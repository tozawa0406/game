//-----------------------------------------------------------------------------
//
//	キーボードInput[KeyInput.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _KEYINPUT_H_
#define _KEYINPUT_H_

#include "Input.h" 

//-----------------------------------------------------------------------------
// クラス宣言
//-----------------------------------------------------------------------------
class KeyInput
{
	friend Input;

public:
	static constexpr int REPEAT_RANGE = 60;		// リピート間隔
	static constexpr int KEY_MAX      = 256;	// キーの最大数

	~KeyInput(void);

	bool Press(int key);
	bool Trig(int key);
	bool Repeat(int key);
	bool Release(int key);

private:
	KeyInput(Input* input);
	HRESULT Init(void);
	void    Update(void);

	Input*               input_;
	LPDIRECTINPUT8       pInput_;				// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 pDevKeyboard_;			// 入力デバイス(キーボード)へのポインタ

	INPUT_BUTTON         state_[KEY_MAX];
};

#endif