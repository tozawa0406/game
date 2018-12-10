//-----------------------------------------------------------------------------
//
//	ダイアログ[Dialog.h]
//	Auther : 戸澤翔太
//                                                                  2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <Windows.h>
#include "../Graphics/Graphics.h"

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class Dialog
{
public:
	// コンストラクタ
	Dialog(void) : dialog_(nullptr) { type_ = Graphics::Type::UNKNOWN; }

	// ダイアログの生成
	HWND CreateStartUpDialog(HINSTANCE instance, HWND hWnd);

	// ダイアログ操作
	void Show(void) { ShowWindow(dialog_, SW_SHOW); }
	void Hide(void) { ShowWindow(dialog_, SW_HIDE); }

	// グラフィックスAPIの取得
	Graphics::Type GetGraphicsType(void) { return type_; }

	// ダイアログ処理
	static BOOL CALLBACK DialogProc(HWND window, uint message, WPARAM wParam, LPARAM lParam);

private:
	HWND dialog_;

	// CALLBACKでいじるために静的
	static Graphics::Type type_;
};


#endif // _DIALOG_H