//-----------------------------------------------------------------------------
//
//	デバッグ処理[Debug.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Debug.h"
#include "../../Windows/Windows.h"
#include "../GameSystems.h"
#include "GuiManager.h"
#include <stdio.h>
#include <vector>

Debug::Debug(Systems* systems) : Interface(systems), debug(false)
{
	debugPause = false;
#if defined(_DEBUG) || defined(DEBUG)
	gui = new GuiManager(this);
#endif
}

Debug::~Debug(void)
{
#if defined(_DEBUG) || defined(DEBUG)
	DeletePtr(gui);
#endif
}

void Debug::On(void)
{
#ifdef _DEBUG
	//フラグの反転
	debug = !debug;
#endif
}

string Debug::BoolToString(bool judge)
{
	string temp = "false";
#ifdef _DEBUG
	if (judge) { temp = "true"; }
#else
	UNREFERENCED_PARAMETER(judge);
#endif
	return temp;
}

void Debug::Update(void)
{
	if (debug) { gui->Update(); }
}

void Debug::Draw(void) 
{
	if (debug) { gui->Draw(); } 
}
