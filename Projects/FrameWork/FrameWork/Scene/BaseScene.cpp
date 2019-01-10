#include "BaseScene.h"
#include "../Systems/GameSystems.h"
#include "SceneManager.h"

BaseScene::BaseScene(void) : 
	manager_(nullptr)
	, systems_(nullptr)
{
}

BaseScene::~BaseScene(void)
{
}

void BaseScene::SetManager(SceneManager* manager)
{
	manager_ = manager;
	if (manager)
	{
		systems_ = manager->GetSystems();
	}
}

Controller* BaseScene::GetCtrl(int i)
{
	if (systems_)
	{
		if (const auto& input = systems_->GetInput())
		{
			return input->GetCtrl(i);
		}
	}
	return nullptr;
}

Sound* BaseScene::GetSound(void)
{
	if (systems_) { return systems_->GetSound(); }

	return nullptr;
}
