//-----------------------------------------------------------------------------
//
//	�L�[�{�[�hInput[KeyInput.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "KeyInput.h"
#include "../../Windows/Windows.h"
#include "../GameSystems.h"
#include "Input.h"

KeyInput::KeyInput(Input* input) : input_(input), pInput_(nullptr), pDevKeyboard_(nullptr)
{
}

// �L�[�{�[�h�̏�����
HRESULT KeyInput::Init(void)
{
	Windows* window = input_->GetSystems()->GetWindow();

	HRESULT hr;
	//���Ŏg���ĂȂ��ϐ��̃��[�j���O��������
	UNREFERENCED_PARAMETER(window->GetHWND());

	// ���͏����̏�����
	if (pInput_ == nullptr)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(window->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pInput_, NULL);
		if (window->ErrorMessage("DirectInput�I�u�W�F�N�g�����܂���", "�G���[", hr))
		{
			return E_FAIL;
		}
	}
	
	// �f�o�C�X�̍쐬
	hr = pInput_->CreateDevice(GUID_SysKeyboard, &pDevKeyboard_, NULL);
	if(window->ErrorMessage("�L�[�{�[�h������܂���", "�G���[", hr))
	{ return E_FAIL; }

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = pDevKeyboard_->SetDataFormat(&c_dfDIKeyboard);
	if(window->ErrorMessage("�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�G���[", hr))
	{ return E_FAIL; }

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = pDevKeyboard_->SetCooperativeLevel(window->GetHWND(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(window->ErrorMessage("�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�G���[", hr))
	{ return E_FAIL; }

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	pDevKeyboard_->Acquire();

	ZeroMemory(state_, sizeof(INPUT_BUTTON) * KEY_MAX);

	return S_OK;
}

// �L�[�{�[�h�̏I������
KeyInput::~KeyInput(void)
{
	if(pDevKeyboard_ != nullptr)
	{// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		pDevKeyboard_->Unacquire();

		pDevKeyboard_->Release();
		pDevKeyboard_ = nullptr;
	}
}

// �L�[�{�[�h�̍X�V����
void KeyInput::Update(void)
{
	byte newKey[KEY_MAX];

	// �f�o�C�X����f�[�^���擾
	if(SUCCEEDED(pDevKeyboard_->GetDeviceState(sizeof(newKey), newKey)))
	{
		for(int keyCnt = 0; keyCnt < KEY_MAX; keyCnt++)
		{
			// �L�[�g���K�[�E�����[�X���𐶐�
			state_[keyCnt].trigger = (state_[keyCnt].press ^ newKey[keyCnt]) & newKey[keyCnt];
			//											(�ߋ�^����) & ����
			//									     1 =  0    1       1
			//										 0 =  1    1       1
			//										 0 =  1    0       0
			state_[keyCnt].release = (state_[keyCnt].press ^ newKey[keyCnt]) & state_[keyCnt].press;
			//										(�ߋ�^����) & �ߋ�
			//										 1 =  1    0       1
			//										 0 =  0    1       0

			// �L�[���s�[�g���𐶐�
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

			// �L�[�v���X����ۑ�
			state_[keyCnt].press = newKey[keyCnt];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		pDevKeyboard_->Acquire();
	}
}

// �L�[�{�[�h�̃v���X��Ԃ��擾
bool KeyInput::Press(int key)
{
	return (state_[key].press & 0x80) ? true : false;
}

// �L�[�{�[�h�̃g���K�[��Ԃ��擾
bool KeyInput::Trig(int key)
{
	return (state_[key].trigger & 0x80) ? true : false;
}

// �L�[�{�[�h�̃����|�X��Ԃ��擾
bool KeyInput::Release(int key)
{
	return (state_[key].release & 0x80) ? true : false;
}

// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
bool KeyInput::Repeat(int key)
{
	return (state_[key].repeat & 0x80) ? true : false;
}
