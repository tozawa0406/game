#include "ResourceInfoManager.h"
#include <FrameWork/Scene/SceneManager.h>

ResourceInfoManager::ResourceInfoManager(void)
{
}

ResourceInfoManager::~ResourceInfoManager(void)
{
}

const string* ResourceInfoManager::LoadTexture(SceneList state, int& size, int& max) const
{
	int baseMax = static_cast<int>(Resources::Texture::Base::MAX);
	switch (state)
	{
	case SceneList::MAX:
		max = baseMax;
		size = max;
		return &texture_.baseFileName[0];
		break;
	case SceneList::TITLE:
		max = static_cast<int>(Resources::Texture::Title::MAX);
		size = max - baseMax;
		return &texture_.titleFileName[0];
		break;
	case SceneList::CAMP:
		max = static_cast<int>(Resources::Texture::Camp::MAX);
		size = max - baseMax;
		return &texture_.campFileName[0];
		break;
	case SceneList::BUTTLE:
		max = static_cast<int>(Resources::Texture::Buttle::MAX);
		size = max - baseMax;
		return &texture_.buttleFileName[0];
		break;
	case SceneList::RESULT:
		max = static_cast<int>(Resources::Texture::Result::MAX);
		size = max - baseMax;
//		return &texture_.resultFileName[0];
		break;
	default: break;
	}

	return nullptr;
}

const string* ResourceInfoManager::LoadModel(SceneList state, int& size, int& max) const
{
	int baseMax = static_cast<int>(Resources::Model::Base::MAX);
	switch (state)
	{
	case SceneList::MAX:
		max = baseMax;
		size = max;
		return &model_.baseFileName[0];
		break;
	case SceneList::TITLE:
		max = static_cast<int>(Resources::Model::Title::MAX);
		size = max - baseMax;
		//return &model_.titleFileName[0];
		break;
	case SceneList::CAMP:
		max = static_cast<int>(Resources::Model::Camp::MAX);
		size = max - baseMax;
		return &model_.campFileName[0];
		break;
	case SceneList::BUTTLE:
		max = static_cast<int>(Resources::Model::Buttle::MAX);
		size = max - baseMax;
		return &model_.buttleFileName[0];
		break;
	case SceneList::RESULT:
		max = static_cast<int>(Resources::Model::Result::MAX);
		size = max - baseMax;
		//	return &model_.resultFileName[0];
		break;
	default: break;
	}

	return nullptr;
}

const ANIMATION_INFO* ResourceInfoManager::LoadModelAnimation(SceneList state, int& size, int& max) const
{
	int baseMax = static_cast<int>(Resources::Model::Animation::Base::MAX);
	switch (state)
	{
	case SceneList::MAX:
		max = baseMax;
		size = max;
		//return &model_..;
		break;
	case SceneList::TITLE:
		max = static_cast<int>(Resources::Model::Animation::Title::MAX);
		size = max - baseMax;
		//return &model_.titleFileName[0];
		break;
	case SceneList::CAMP:
		max = static_cast<int>(Resources::Model::Animation::Camp::MAX);
		size = max - baseMax;
		return &model_.animationCampFileName[0];
		break;
	case SceneList::BUTTLE:
		max = static_cast<int>(Resources::Model::Animation::Buttle::MAX);
		size = max - baseMax;
		return &model_.animationButtleFileName[0];
		break;
	case SceneList::RESULT:
		max = static_cast<int>(Resources::Model::Animation::Result::MAX);
		size = max - baseMax;
		//return &model_.resultFileName[0];
		break;
	default: break;
	}

	return nullptr;
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
		break;
	case SceneList::TITLE:
		max  = static_cast<int>(Resources::Sound::Title::MAX);
		size = max - baseMax;
		return &sound_.titleFileName[0];
		break;
	case SceneList::CAMP:
		max = static_cast<int>(Resources::Sound::Camp::MAX);
		size = max - baseMax;
		return &sound_.campFileName[0];
		break;
	case SceneList::BUTTLE:
		max = static_cast<int>(Resources::Sound::Buttle::MAX);
		size = max - baseMax;
		return &sound_.buttleFileName[0];
		break;
	case SceneList::RESULT:
		max = static_cast<int>(Resources::Sound::Result::MAX);
		size = max - baseMax;
		return &sound_.resultFileName[0];
		break;
	default: break;
	}

	return nullptr;
}

const string& ResourceInfoManager::GetWhiteTextureName(void) const
{
	return texture_.baseFileName[0];
}
