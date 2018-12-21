#include "Windows.h"
#include <Windows.h>
#include "../Graphics/DirectX11/DirectX11.h"
#include "../Systems/GameSystems.h"
#include "resource.h"

#include <time.h>
#include <tchar.h>

Windows::Windows(void) : 
	graphics_(nullptr)
	, systems_(nullptr)
	, hWnd_(nullptr)
	, hInstance_(nullptr)
	, timeNow_(0)
	, timeOld_(0)
	, timeFPS_(0)
	, fpsCnt_(0)
	, fps_(0)
{
}

Windows::~Windows(void)
{
}

HRESULT Windows::Init(HINSTANCE Instance, int cmdShow)
{
	// �������[���[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	hInstance_ = Instance;

	// �E�B���h�E���̐ݒ�
	SetWindowInfo();

	// �E�B���h�E�̐���
	SetWindow(cmdShow);

	return S_OK;
}

WPARAM Windows::Uninit(void)
{
	UninitDeletePtr(systems_);
	UninitDeletePtr(graphics_);

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex_.hInstance);

	return msg_.wParam;
}

bool Windows::Update(void)
{
	if (!graphics_)
	{
		// �_�C�A���O���Ȃ��ꍇ�͎w��ʂ�ɐ���
		return InitGame(graphicsType_);
	}

	// �V�X�e���n�̍X�V����
	if (systems_) { systems_->Update(); }

	return false;
}

void Windows::Draw(void)
{
	if (!graphics_ || !systems_) { return; }

	if (SUCCEEDED(graphics_->DrawBegin()))
	{
		// �V�X�e���n�̕`��
		systems_->Draw();

		graphics_->DrawEnd();
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam);
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
			// �ÓI�ϐ��ł͂Ȃ��̂ł��̌`��
			if (const auto& systems = Systems::Instance())
			{
				if (const auto& debug = systems->GetDebug())
				{
					debug->On();
				}
			}
			break;
		case VK_F3:
			// �ÓI�ϐ��ł͂Ȃ��̂ł��̌`��
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

bool Windows::ErrorMessage(const char* message, char* error, HRESULT hr)
{
	if (FAILED(hr))
	{
		MessageBox(hWnd_, message, error, MB_ICONWARNING);
		return true;
	}
	return false;
}

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

void Windows::SetWindow(int cmdShow)
{
	RECT dr;
	GetWindowRect(GetDesktopWindow(), &dr);

	int w = WIDTH;
	int h = HEIGHT;

	RECT wr = { 0, 0, w, h };
	AdjustWindowRect(&wr, WINDOW_STYLE, false);
	int nWidth  = wr.right  - wr.left;
	int	nHeight = wr.bottom - wr.top;

	hWnd_ = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WINDOW_STYLE, WindowCenter(dr.right, wr.right), WindowCenter(dr.bottom, wr.bottom),
		nWidth, nHeight, NULL, NULL, hInstance_, NULL);

	// �E�B���h�E�\��
	ShowWindow(hWnd_, cmdShow);

	// �E�B���h�E�X�V
	UpdateWindow(hWnd_);
}

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

void Windows::GameLoop(DWORD fps)
{
	// timeGetTIme�̐��x���グ��
	timeBeginPeriod(1);

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
			timeNow_ = timeGetTime();

			if ((timeNow_ - timeFPS_) >= 500)
			{
				fps_ = fpsCnt_ * 1000.f / (timeNow_ - timeFPS_);
				timeFPS_ = timeNow_;
				fpsCnt_ = 0;
			}

			if (((timeNow_ - timeOld_) * fps) >= 1000)
			{
				// �X�V����
				if (Update()) { break; }

				// �`�揈��
				Draw();

				timeOld_ = timeNow_;
				fpsCnt_++;
			}
		}
	}

	timeEndPeriod(1);
}

bool Windows::InitGame(Graphics::Type type)
{
	if (type == Graphics::Type::UNKNOWN) { return false; }
	else { graphicsType_ = type; }

	// �O���t�B�b�N�XAPI�̐���
	if (graphicsType_ == Graphics::Type::DirectX11) 
	{
		graphics_ = new DirectX11; 
	}
	else
	{
		ErrorMessage("���̃A�v���P�[�V�������Ή����Ă��Ȃ��O���t�B�b�N�XAPI�ł��B", "�G���[", E_FAIL);
		return true;
	}
	
	if (graphics_)
	{
		graphics_->SetWindows(this);
		if (FAILED(graphics_->Init())) { return true; }
	}

	// �V�X�e���n�̐���
	systems_ = new Systems(this);
	if (systems_)
	{
		if (FAILED(systems_->Init())) { return true; }
	}

	return false;
}
