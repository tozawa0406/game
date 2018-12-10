//-----------------------------------------------------------------------------
//
//	Window�̏����S��[Windows.h]
//	Auther : ���V�đ�
//                                                                  2018/05/10
//-----------------------------------------------------------------------------
#ifndef _WINDOWS_H_
#define _WINDOWS_H_

#include "../Define/Define.h"
#include "../Graphics/Graphics.h"

#include "Dialog.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Xinput.lib")

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class Systems;
class Windows
{
	// �萔��`
	static constexpr char* CLASS_NAME   = "Game";			// �E�C���h�E�̖��O
	static constexpr char* WINDOW_NAME  = "Game";			// �E�C���h�E�̖��O
	static constexpr int   WINDOW_STYLE = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX;	// �E�B���h�E�X�^�C��

public:
	// �萔��`
	static Graphics::Type GRAPHICS_TYPE;
	static constexpr bool FULL_SCREEN	= false;
	static constexpr int  FPS			= 60;					// FPS�̐ݒ�
	// �E�B���h�E�̃T�C�Y
	static constexpr int  WIDTH			= 1280;
	static constexpr int  HEIGHT		= 720;

	Windows(void);
	~Windows(void) {}

	// Getter
	float     GetFps(void)        { return fps_;        }	// ���ۂ�FPS
	HWND      GetHWND(void)       { return hWnd_;       }	// HWND
	MSG       GetMsg(void)        { return msg_;        }	// MSG
	HINSTANCE GetHInstance(void)  { return hInstance_;  }	// HINSTANCE

	Graphics* GetGraphics(void)   { return graphics_;   }	// �q
	Systems*  GetSystems(void)    { return systems_;    }	// �q

	// main�ŌĂяo�����
	HRESULT Init(HINSTANCE Instance, int cmdShow);		// ����������
	void    GameLoop(DWORD fps);						// �Q�[�����[�v
	WPARAM  Uninit(void);								// �㏈��

	// �G���[���b�Z�[�W
	bool ErrorMessage(const char* message, char* error, HRESULT hr);

private:
	void    SetWindowInfo(void);				// �E�B���h�E���̐ݒ�
	void    SetWindow(int cmdShow);				// �E�B���h�E�̐���
	bool    MsgProcess(void);					// ���b�Z�[�W�v���Z�X
	bool    Update(void);						// �X�V����
	void    Draw(void);							// �`�揈��
	bool	InitGame(Graphics::Type type);		// �Q�[���n�̏���������

	inline LONG WindowCenter(LONG dr, LONG wr) { return Half(dr - wr) > 0 ? Half(dr - wr) : 0; }

	Graphics*	graphics_;
	Systems*	systems_;			// �V�X�e���n
	//Dialog*		dialog_;

	HWND		hWnd_;
	HINSTANCE	hInstance_;
	MSG			msg_;
	WNDCLASSEX	wcex_;

	int			timeNow_;		//
	int			timeOld_;		// ���t���[���ȍ~�Ɏg��
	int			timeFPS_;
	int			fpsCnt_;
	float		fps_;

	//�E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WndProc(HWND hWnd, uint uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // _WINDOWS_H