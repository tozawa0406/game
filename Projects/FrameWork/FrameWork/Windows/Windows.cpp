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
	// メモリーリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	hInstance_ = Instance;

	// ウィンドウ情報の設定
	SetWindowInfo();

	// ウィンドウの生成
	SetWindow(cmdShow);

	return S_OK;
}

WPARAM Windows::Uninit(void)
{
	UninitDeletePtr(systems_);
	UninitDeletePtr(graphics_);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex_.hInstance);

	return msg_.wParam;
}

bool Windows::Update(void)
{
	if (!graphics_)
	{
		// ダイアログがない場合は指定通りに生成
		return InitGame(graphicsType_);
	}

	// システム系の更新処理
	if (systems_) { systems_->Update(); }

	return false;
}

void Windows::Draw(void)
{
	if (!graphics_ || !systems_) { return; }

	if (SUCCEEDED(graphics_->DrawBegin()))
	{
		// システム系の描画
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
		case VK_ESCAPE:			// [ESC]キーが押されたら
		{
			int nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		case VK_F1:
			// 静的変数ではないのでこの形で
			if (const auto& systems = Systems::Instance())
			{
				if (const auto& debug = systems->GetDebug())
				{
					debug->On();
				}
			}
			break;
		case VK_F3:
			// 静的変数ではないのでこの形で
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
	wcex_.cbSize        = sizeof(WNDCLASSEX);			// 構造体WNDCLASSEXの大きさ
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

	RegisterClassEx(&wcex_);			// 登録

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

	// ウィンドウ表示
	ShowWindow(hWnd_, cmdShow);

	// ウィンドウ更新
	UpdateWindow(hWnd_);
}

bool Windows::MsgProcess(void)
{
	//ウィンドウを閉じた時
	if (msg_.message == WM_QUIT)
	{
		return true;
	}
	//それ以外
	else
	{
		TranslateMessage(&msg_);
		DispatchMessage(&msg_);
	}
	return false;
}

void Windows::GameLoop(DWORD fps)
{
	// timeGetTImeの精度を上げる
	timeBeginPeriod(1);

	for (;;)
	{
		//メッセージ処理
		if (PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE))
		{
			if (MsgProcess()) { break; }
		}
		//ゲーム処理
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
				// 更新処理
				if (Update()) { break; }

				// 描画処理
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

	// グラフィックスAPIの生成
	if (graphicsType_ == Graphics::Type::DirectX11) 
	{
		graphics_ = new DirectX11; 
	}
	else
	{
		ErrorMessage("このアプリケーションが対応していないグラフィックスAPIです。", "エラー", E_FAIL);
		return true;
	}
	
	if (graphics_)
	{
		graphics_->SetWindows(this);
		if (FAILED(graphics_->Init())) { return true; }
	}

	// システム系の生成
	systems_ = new Systems(this);
	if (systems_)
	{
		if (FAILED(systems_->Init())) { return true; }
	}

	return false;
}
