//-----------------------------------------------------------------------------
//
//	�_�C�A���O[Dialog.h]
//	Auther : ���V�đ�
//                                                                  2018/08/18
//-----------------------------------------------------------------------------
#include "Dialog.h"
#include "resource.h"

// CALLBACK�ł����邽�߂ɐÓI
Graphics::Type Dialog::type_ = Graphics::Type::UNKNOWN;

// �_�C�A���O�̐���
HWND Dialog::CreateStartUpDialog(HINSTANCE instance, HWND hWnd)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(hWnd);
	// �_�C�A���O�̐���
//	dialog_ = CreateDialog(instance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);
	Show();
	return dialog_;
}

// �_�C�A���O����
BOOL Dialog::DialogProc(HWND window, uint message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
	{
		// �_�C�A���O�̃{�^���ɉ���������
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
