//-----------------------------------------------------------------------------
//
//	Windowの処理全般[Windows.cpp]
//	Auther : 戸澤翔太
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

// コンストラクタ
Windows::Windows(void) : graphics_(nullptr), systems_(nullptr)//, dialog_(nullptr)
					   , hWnd_(nullptr), hInstance_(nullptr)
					   , timeNow_(0), timeOld_(0), timeFPS_(0), fpsCnt_(0), fps_(0)
{
}

// 初期化処理
HRESULT Windows::Init(HINSTANCE Instance, int cmdShow)
{
	hInstance_ = Instance;

	// ウィンドウ情報の設定
	SetWindowInfo();

	// ウィンドウの生成
	SetWindow(cmdShow);

	// ダイアログの生成
	//dialog_ = new Dialog;
	//dialog_->CreateStartUpDialog(hInstance_, hWnd_);

	// レンダラー初期化
	graphics_ = nullptr;

	// ゲームシステムの初期化
	systems_ = nullptr;

	return S_OK;
}

// 後処理
WPARAM Windows::Uninit(void)
{
	// システム系の後処理
	if (systems_)
	{
		systems_->Uninit();
	}
	DeletePtr(systems_);

	// グラフィックスAPIの後処理
	if (graphics_)
	{
		graphics_->Uninit();
	}
	DeletePtr(graphics_);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex_.hInstance);

	return msg_.wParam;
}

// 更新処理
bool Windows::Update(void)
{
	if (graphics_ == nullptr) 
	{
		return InitGame(GRAPHICS_TYPE/*dialog_->GetGraphicsType()*/);
	}
	// システム系の更新処理
	systems_->Update();

	return false;
}

// 描画処理
void Windows::Draw(void)
{
	if (graphics_ == nullptr) { return; }
	if (SUCCEEDED(graphics_->DrawBegin()))
	{
		// システム系の描画
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

// エラーメッセージ
bool Windows::ErrorMessage(const char* message, char* error, HRESULT hr)
{
	if (FAILED(hr))
	{
		MessageBox(hWnd_, message, error, MB_ICONWARNING);
		return true;
	}
	return false;
}

// ウィンドウ情報の設定
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

// ウィンドウの生成
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

	ShowWindow(hWnd_, cmdShow);		// ウィンドウ表示

	UpdateWindow(hWnd_);			// ウィンドウ更新
}

// メッセージプロセス
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

// ゲームループ
void Windows::GameLoop(DWORD fps)
{
	timeBeginPeriod(1);				// timeGetTImeの精度を上げる

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
			// ゲーム更新処理
			timeNow_ = timeGetTime();

			if ((timeNow_ - timeFPS_) >= 500)
			{
				fps_ = fpsCnt_ * 1000.f / (timeNow_ - timeFPS_);
				timeFPS_ = timeNow_;
				fpsCnt_ = 0;
			}

			if (((timeNow_ - timeOld_) * fps) >= 1000)
			{// 更新、描画

				if (this->Update()) { break; }

				// 描画処理
				this->Draw();

				timeOld_ = timeNow_;
				fpsCnt_++;
			}
		}
	}

	timeEndPeriod(1);
}

// ゲーム系の初期化処理
bool Windows::InitGame(Graphics::Type type)
{
	if (type == Graphics::Type::UNKNOWN) { return false; }
	else { GRAPHICS_TYPE = type; }

	//string temp = WINDOW_NAME;
	//if      (RENDERER_TYPE == Graphics::Type::DirectX9)  { temp += " DirectX9";  }
	//else if (RENDERER_TYPE == Graphics::Type::DirectX11) { temp += " DirectX11"; }
	//else if (RENDERER_TYPE == Graphics::Type::OpenGL)    { temp += " OpenGL";    }

	// ウィンドウ名の変更
	//SetWindowText(hWnd_, temp.c_str());

	// ダイアログの消去
	//DeletePtr(dialog_);

	// グラフィックスAPIの生成
	if(GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{ 
		graphics_ = new DirectX11(this);
	}
	if (FAILED(graphics_->Init())) { return true; }

	// システム系の生成
	systems_ = new Systems(this);
	if (FAILED(systems_->Init()))  { return true; }

	return false;
}
