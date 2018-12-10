//-----------------------------------------------------------------------------
//
//	Window�̏����S��[Windows.cpp]
//	Auther : ���V�đ�
//                                                                  2018/05/10
//-----------------------------------------------------------------------------
#include "Windows.h"
#include <Windows.h>
#include "../Graphics/DirectX11/DirectX11.h"
#include "../Systems/GameSystems.h"
#include "resource.h"

#include <time.h>
#include <tchar.h>

Graphics::Type Windows::GRAPHICS_TYPE = Graphics::Type::DirectX11;

// �R���X�g���N�^
Windows::Windows(void) : graphics_(nullptr), systems_(nullptr)//, dialog_(nullptr)
					   , hWnd_(nullptr), hInstance_(nullptr)
					   , timeNow_(0), timeOld_(0), timeFPS_(0), fpsCnt_(0), fps_(0)
{
}

// ����������
HRESULT Windows::Init(HINSTANCE Instance, int cmdShow)
{
	hInstance_ = Instance;

	// �E�B���h�E���̐ݒ�
	SetWindowInfo();

	// �E�B���h�E�̐���
	SetWindow(cmdShow);

	// �_�C�A���O�̐���
	//dialog_ = new Dialog;
	//dialog_->CreateStartUpDialog(hInstance_, hWnd_);

	// �����_���[������
	graphics_ = nullptr;

	// �Q�[���V�X�e���̏�����
	systems_ = nullptr;

	return S_OK;
}

// �㏈��
WPARAM Windows::Uninit(void)
{
	// �V�X�e���n�̌㏈��
	if (systems_)
	{
		systems_->Uninit();
	}
	DeletePtr(systems_);

	// �O���t�B�b�N�XAPI�̌㏈��
	if (graphics_)
	{
		graphics_->Uninit();
	}
	DeletePtr(graphics_);

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex_.hInstance);

	return msg_.wParam;
}

// �X�V����
bool Windows::Update(void)
{
	if (graphics_ == nullptr) 
	{
		return InitGame(GRAPHICS_TYPE/*dialog_->GetGraphicsType()*/);
	}
	// �V�X�e���n�̍X�V����
	systems_->Update();

	return false;
}

// �`�揈��
void Windows::Draw(void)
{
	if (graphics_ == nullptr) { return; }
	if (SUCCEEDED(graphics_->DrawBegin()))
	{
		// �V�X�e���n�̕`��
		systems_->Draw();

		graphics_->DrawEnd();
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);
// WndProc
LRESULT CALLBACK Windows::WndProc(HWND hWnd, uint uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) { return true; }

	switch (uMsg)
	{
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
		{
			return 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:			// [ESC]�L�[�������ꂽ��
		{
			int nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_DEFBUTTON2);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		case VK_F1:
			if (const auto& systems = Systems::Instance())
			{
				if (const auto& debug = systems->GetDebug())
				{
					debug->On();
				}
			}
			break;
		case VK_F3:
			if (const auto& systems = Systems::Instance())
			{
				if (const auto& debug = systems->GetDebug())
				{
					debug->SetDebugPause(!debug->GetDebugPause());
				}
			}
			break;
		default:
			break;
		}
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// �G���[���b�Z�[�W
bool Windows::ErrorMessage(const char* message, char* error, HRESULT hr)
{
	if (FAILED(hr))
	{
		MessageBox(hWnd_, message, error, MB_ICONWARNING);
		return true;
	}
	return false;
}

// �E�B���h�E���̐ݒ�
void Windows::SetWindowInfo(void)
{
	wcex_.cbSize        = sizeof(WNDCLASSEX);			// �\����WNDCLASSEX�̑傫��
	wcex_.style         = CS_VREDRAW | CS_HREDRAW;
	wcex_.lpfnWndProc   = WndProc;
	wcex_.cbClsExtra    = 0;
	wcex_.cbWndExtra    = 0;
	wcex_.hInstance     = hInstance_;
//	wcex_.hIcon			= LoadIcon(hInstance_, MAKEINTRESOURCE(IDI_ICON1));
	wcex_.hIcon			= NULL;
	wcex_.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex_.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex_.lpszMenuName  = NULL;
	wcex_.lpszClassName = CLASS_NAME;
	wcex_.hIconSm       = NULL;

	RegisterClassEx(&wcex_);			// �o�^

	UnregisterClass(_T("ImGui Example"), wcex_.hInstance);
}

// �E�B���h�E�̐���
void Windows::SetWindow(int cmdShow)
{
	RECT dr;
	GetWindowRect(GetDesktopWindow(), &dr);

	int w = WIDTH;
	int h = HEIGHT;

	//if (FULL_SCREEN)
	//{
	//	w = dr.right;
	//	float aspect = (float)w / 16;
	//	h = (int)(aspect * 9);
	//}

	RECT wr = { 0, 0, w, h };
	AdjustWindowRect(&wr, WINDOW_STYLE, false);
	int nWidth  = wr.right  - wr.left;
	int	nHeight = wr.bottom - wr.top;

	hWnd_ = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WINDOW_STYLE, WindowCenter(dr.right, wr.right), WindowCenter(dr.bottom, wr.bottom),
		nWidth, nHeight, NULL, NULL, hInstance_, NULL);

	ShowWindow(hWnd_, cmdShow);		// �E�B���h�E�\��

	UpdateWindow(hWnd_);			// �E�B���h�E�X�V
}

// ���b�Z�[�W�v���Z�X
bool Windows::MsgProcess(void)
{
	//�E�B���h�E�������
	if (msg_.message == WM_QUIT)
	{
		return true;
	}
	//����ȊO
	else
	{
		TranslateMessage(&msg_);
		DispatchMessage(&msg_);
	}
	return false;
}

// �Q�[�����[�v
void Windows::GameLoop(DWORD fps)
{
	timeBeginPeriod(1);				// timeGetTIme�̐��x���グ��

	for (;;)
	{
		//���b�Z�[�W����
		if (PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE))
		{
			if (MsgProcess()) { break; }
		}
		//�Q�[������
		else
		{
			// �Q�[���X�V����
			timeNow_ = timeGetTime();

			if ((timeNow_ - timeFPS_) >= 500)
			{
				fps_ = fpsCnt_ * 1000.f / (timeNow_ - timeFPS_);
				timeFPS_ = timeNow_;
				fpsCnt_ = 0;
			}

			if (((timeNow_ - timeOld_) * fps) >= 1000)
			{// �X�V�A�`��

				if (this->Update()) { break; }

				// �`�揈��
				this->Draw();

				timeOld_ = timeNow_;
				fpsCnt_++;
			}
		}
	}

	timeEndPeriod(1);
}

// �Q�[���n�̏���������
bool Windows::InitGame(Graphics::Type type)
{
	if (type == Graphics::Type::UNKNOWN) { return false; }
	else { GRAPHICS_TYPE = type; }

	//string temp = WINDOW_NAME;
	//if      (RENDERER_TYPE == Graphics::Type::DirectX9)  { temp += " DirectX9";  }
	//else if (RENDERER_TYPE == Graphics::Type::DirectX11) { temp += " DirectX11"; }
	//else if (RENDERER_TYPE == Graphics::Type::OpenGL)    { temp += " OpenGL";    }

	// �E�B���h�E���̕ύX
	//SetWindowText(hWnd_, temp.c_str());

	// �_�C�A���O�̏���
	//DeletePtr(dialog_);

	// �O���t�B�b�N�XAPI�̐���
	if(GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{ 
		graphics_ = new DirectX11(this);
	}
	if (FAILED(graphics_->Init())) { return true; }

	// �V�X�e���n�̐���
	systems_ = new Systems(this);
	if (FAILED(systems_->Init()))  { return true; }

	return false;
}
