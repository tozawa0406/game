/*
 * @file	main.cpp
 * @brief	���C��
 * @author	���V�đ�
 * @date	2018/10/12
 */
#include <FrameWork/Windows/Windows.h>

// @fn		WinMain
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// �������[���[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Windows window;

	if (FAILED(window.Init(hInstance, nCmdShow))) { return 1; }

	window.GameLoop(Windows::FPS);

	return (int)window.Uninit();
}