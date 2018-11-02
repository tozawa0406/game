//-----------------------------------------------------------------------------
//
//	GUI[GUI.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _GUI_H_
#define _GUI_H_

#include "../../Define/Define.h"
#include "../../Object/Object.h"
#include "ImGUI/imgui.h"

class Systems;
class GUI
{
	friend  class GuiManager;
public:
	GUI(Systems* systems, Object* obj, string name = "");
	~GUI(void);
	virtual void GuiUpdate(void) {};

protected:
	string name;

private:
	Object* obj;
	string tag;
	bool    window = false;
	int     drawNum;
	GuiManager* guiManager;
};

#endif // _GUI_H_
