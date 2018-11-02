//-----------------------------------------------------------------------------
//
//	ƒV[ƒ“Šî’ê[BaseScene.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/02/27
//-----------------------------------------------------------------------------
#include "BaseScene.h"
#include "../Systems/GameSystems.h"
#include "SceneManager.h"

BaseScene::BaseScene(SceneManager* manager)
{
	manager_ = manager;
	systems_ = manager->GetSystems();
}

Controller* BaseScene::GetCtrl(int i)
{
	return systems_->GetInput()->GetCtrl(i);
}
