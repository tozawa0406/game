//-----------------------------------------------------------------------------
//
//	GUI[GUI.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "GUI.h"
#include "../GameSystems.h"
#include "GuiManager.h"

GUI::GUI(Systems* systems, Object* obj, string name) : guiManager(nullptr)
{
#ifdef _SELF_DEBUG
	this->obj  = obj;
	this->name = name;
	this->tag  = name;

	if (systems)
	{
		this->guiManager = systems->GetDebug()->GetGuiManager();
		guiManager->Add(this);
	}
#else
	UNREFERENCED_PARAMETER(systems);
	UNREFERENCED_PARAMETER(obj);
#endif
}

//�f�X�g���N�^
GUI::~GUI(void)
{
#ifdef _SELF_DEBUG
	guiManager->Remove(this);
#endif
}
