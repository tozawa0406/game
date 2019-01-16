#include "ResourceInfoManager.h"
#include <FrameWork/Scene/SceneManager.h>

ResourceInfoManager::ResourceInfoManager(void)
{
}

ResourceInfoManager::~ResourceInfoManager(void)
{
}

const SOUNDPARAM* ResourceInfoManager::LoadSound(SceneList state, int& size, int& max) const
{
	int baseMax = static_cast<int>(Resources::Sound::Base::MAX);
	switch (state)
	{
	case SceneList::MAX:
		max  = baseMax;
		size = max;
		return &sound_.baseFileName[0];
	case SceneList::TITLE:
		max  = static_cast<int>(Resources::Sound::Title::MAX);
		size = max - baseMax;
		return &sound_.titleFileName[0];
	case SceneList::CAMP:
		max = static_cast<int>(Resources::Sound::Camp::MAX);
		size = max - baseMax;
		return &sound_.campFileName[0];
	case SceneList::BUTTLE:
		max = static_cast<int>(Resources::Sound::Buttle::MAX);
		size = max - baseMax;
		return &sound_.buttleFileName[0];
	case SceneList::RESULT:
		max = static_cast<int>(Resources::Sound::Result::MAX);
		size = max - baseMax;
		return &sound_.resultFileName[0];
	default: break;
	}

	return nullptr;
}
