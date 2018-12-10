//-----------------------------------------------------------------------------
//
//	�_�C�A���O[Dialog.h]
//	Auther : ���V�đ�
//                                                                  2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <Windows.h>
#include "../Graphics/Graphics.h"

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class Dialog
{
public:
	// �R���X�g���N�^
	Dialog(void) : dialog_(nullptr) { type_ = Graphics::Type::UNKNOWN; }

	// �_�C�A���O�̐���
	HWND CreateStartUpDialog(HINSTANCE instance, HWND hWnd);

	// �_�C�A���O����
	void Show(void) { ShowWindow(dialog_, SW_SHOW); }
	void Hide(void) { ShowWindow(dialog_, SW_HIDE); }

	// �O���t�B�b�N�XAPI�̎擾
	Graphics::Type GetGraphicsType(void) { return type_; }

	// �_�C�A���O����
	static BOOL CALLBACK DialogProc(HWND window, uint message, WPARAM wParam, LPARAM lParam);

private:
	HWND dialog_;

	// CALLBACK�ł����邽�߂ɐÓI
	static Graphics::Type type_;
};


#endif // _DIALOG_H