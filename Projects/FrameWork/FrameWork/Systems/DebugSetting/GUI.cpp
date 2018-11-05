//-----------------------------------------------------------------------------
//
//	GUI[GUI.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "GUI.h"
#include "../GameSystems.h"
#include "GuiManager.h"

GUI::GUI(Systems* systems, Object* obj, string name)
{
#ifdef _SELF_DEBUG
	this->obj  = obj;
	this->name = name;
	this->tag  = name;

	this->guiManager = systems->GetDebug()->GetGuiManager();
	guiManager->gui.push_back(this);
#else
	UNREFERENCED_PARAMETER(systems);
	UNREFERENCED_PARAMETER(obj);
#endif
}

//デストラクタ
GUI::~GUI(void)
{
#ifdef _SELF_DEBUG
	RemoveVector(guiManager->gui, this);
#endif
}
