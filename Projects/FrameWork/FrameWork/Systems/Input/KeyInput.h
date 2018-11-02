//-----------------------------------------------------------------------------
//
//	�L�[�{�[�hInput[KeyInput.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _KEYINPUT_H_
#define _KEYINPUT_H_

#include "Input.h" 

//-----------------------------------------------------------------------------
// �N���X�錾
//-----------------------------------------------------------------------------
class KeyInput
{
	friend Input;

public:
	static constexpr int REPEAT_RANGE = 60;		// ���s�[�g�Ԋu
	static constexpr int KEY_MAX      = 256;	// �L�[�̍ő吔

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
	LPDIRECTINPUT8       pInput_;				// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 pDevKeyboard_;			// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

	INPUT_BUTTON         state_[KEY_MAX];
};

#endif