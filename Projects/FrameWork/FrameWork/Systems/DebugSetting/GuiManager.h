//-----------------------------------------------------------------------------
//
//	GUIä«óù[GuiManager.h]
//	Auther : åÀ‡V„ƒëæ
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "../../Define/Define.h"
#include "../../Windows/Windows.h"
#include "ImGUI/imgui.h"
#include "GUI.h"

class Systems;
class Debug;
class GuiManager
{
	static constexpr int   WINDOW_MARGIN_X = 20;
	static constexpr int   WINDOW_MARGIN_Y = 20;
	static constexpr float WINDOW_WIDTH  = ((float)Windows::WIDTH  - WINDOW_MARGIN_X * 6) / 5;
	static constexpr float WINDOW_HEIGHT = ((float)Windows::HEIGHT - WINDOW_MARGIN_Y * 6) / 5;
public:
	static constexpr int FLASHING   = 10;
	static constexpr int SELECT_GUI = 30;

	GuiManager(Debug* debug);
	~GuiManager(void);
	void GuiUpdate(void);
	void Update(void);
	void Draw(void);
	const VECTOR3* GetSelect(void);
	Debug* GetDebug(void) { return debug; }

	bool GetLookObject(void) { return lookObject; }

private:
	Debug* debug;
	std::vector<GUI*> gui;
	friend GUI;

	bool guiObject  = false;
	bool guiSubject = false;

	bool lookObject = false;
	bool draw_;

	int drawNum = 0;
	int frame = 0;
};

#endif // _GUI_MANAGER_H_
