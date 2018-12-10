//-----------------------------------------------------------------------------
//
//	ダイアログ[Dialog.h]
//	Auther : 戸澤翔太
//                                                                  2018/08/18
//-----------------------------------------------------------------------------
#include "Dialog.h"
#include "resource.h"

// CALLBACKでいじるために静的
Graphics::Type Dialog::type_ = Graphics::Type::UNKNOWN;

// ダイアログの生成
HWND Dialog::CreateStartUpDialog(HINSTANCE instance, HWND hWnd)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(hWnd);
	// ダイアログの生成
//	dialog_ = CreateDialog(instance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);
	Show();
	return dialog_;
}

// ダイアログ処理
BOOL Dialog::DialogProc(HWND window, uint message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
	{
		// ダイアログのボタンに応じた処理
		int menuID = LOWORD(wParam);
		switch (menuID)
		{
		case IDC_BUTTON1:
			type_ = Graphics::Type::DirectX9;
			DestroyWindow(window);
			break;
		case IDC_BUTTON2:
			type_ = Graphics::Type::DirectX11;
			DestroyWindow(window);
			break;
		case IDC_BUTTON3:
			type_ = Graphics::Type::OpenGL;
			DestroyWindow(window);
			break;
		}
	}
	}

	return FALSE;
}
