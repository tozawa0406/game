/*
 * @file	main.cpp
 * @brief	ÉÅÉCÉì
 * @author	åÀ‡V„ƒëæ
 * @date	2018/10/12
 */
#include <FrameWork/Windows/Windows.h>

/* @brief	main		*/
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Windows window;

	if (FAILED(window.Init(hInstance, nCmdShow))) { return 1; }

	window.GameLoop(Windows::FPS);

	return (int)window.Uninit();
}
