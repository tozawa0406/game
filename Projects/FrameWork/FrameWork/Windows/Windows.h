//-----------------------------------------------------------------------------
//
//	Windowの処理全般[Windows.h]
//	Auther : 戸澤翔太
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
//	クラス定義
//-----------------------------------------------------------------------------
class Systems;
class Windows
{
	// 定数定義
	static constexpr char* CLASS_NAME   = "Game";			// ウインドウの名前
	static constexpr char* WINDOW_NAME  = "Game";			// ウインドウの名前
	static constexpr int   WINDOW_STYLE = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX;	// ウィンドウスタイル

public:
	// 定数定義
	static Graphics::Type GRAPHICS_TYPE;
	static constexpr bool FULL_SCREEN	= false;
	static constexpr int  FPS			= 60;					// FPSの設定
	// ウィンドウのサイズ
	static constexpr int  WIDTH			= 1280;
	static constexpr int  HEIGHT		= 720;

	Windows(void);
	~Windows(void) {}

	// Getter
	float     GetFps(void)        { return fps_;        }	// 実際のFPS
	HWND      GetHWND(void)       { return hWnd_;       }	// HWND
	MSG       GetMsg(void)        { return msg_;        }	// MSG
	HINSTANCE GetHInstance(void)  { return hInstance_;  }	// HINSTANCE

	Graphics* GetGraphics(void)   { return graphics_;   }	// 子
	Systems*  GetSystems(void)    { return systems_;    }	// 子

	// mainで呼び出される
	HRESULT Init(HINSTANCE Instance, int cmdShow);		// 初期化処理
	void    GameLoop(DWORD fps);						// ゲームループ
	WPARAM  Uninit(void);								// 後処理

	// エラーメッセージ
	bool ErrorMessage(const char* message, char* error, HRESULT hr);

private:
	void    SetWindowInfo(void);				// ウィンドウ情報の設定
	void    SetWindow(int cmdShow);				// ウィンドウの生成
	bool    MsgProcess(void);					// メッセージプロセス
	bool    Update(void);						// 更新処理
	void    Draw(void);							// 描画処理
	bool	InitGame(Graphics::Type type);		// ゲーム系の初期化処理

	inline LONG WindowCenter(LONG dr, LONG wr) { return Half(dr - wr) > 0 ? Half(dr - wr) : 0; }

	Graphics*	graphics_;
	Systems*	systems_;			// システム系
	//Dialog*		dialog_;

	HWND		hWnd_;
	HINSTANCE	hInstance_;
	MSG			msg_;
	WNDCLASSEX	wcex_;

	int			timeNow_;		//
	int			timeOld_;		// 次フレーム以降に使う
	int			timeFPS_;
	int			fpsCnt_;
	float		fps_;

	//ウィンドウプロシージャ
	static LRESULT CALLBACK WndProc(HWND hWnd, uint uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // _WINDOWS_H