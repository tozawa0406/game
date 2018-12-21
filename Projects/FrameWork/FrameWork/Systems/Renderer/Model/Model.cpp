//-----------------------------------------------------------------------------
//
//	モデル管理[Model.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Model.h"
#include "../../../Systems/GameSystems.h"
#include "../../../Graphics/Graphics.h"
#include "../../../Graphics/Wrapper.h"
#include "../../../Scene/BaseScene.h"
#include "../../Loading.h"
#include "../../../../../Sources/Scene/EachScene.h"

Model::Model(Systems* systems) : Interface(systems)
{
}

HRESULT Model::Init(void)
{
	int size = (int)Base::MAX;
	for (int i = 0; i < size; ++i)
	{
		// 現状は存在しないのでコメントアウト
//		HRESULT hr = systems_->GetGraphics()->GetWrapper()->LoadModel(baseFileName[i], i);
//		if (FAILED(hr)) { return E_FAIL; }
	}

	return S_OK;
}

void Model::Uninit(void)
{
	Release(true);
}

int	Model::SetUpLoading(Loading* loading, int sceneNum)
{
	loading_ = loading;

	int size = 0;

	switch ((SceneList)sceneNum)
	{
	case SceneList::TITLE:
		size = (int)Title::MAX - (int)Base::MAX;
		break;
	case SceneList::GAME:
		size = (int)Game::MAX - (int)Base::MAX;
		break;
	case SceneList::RESULT:
		size = (int)Result::MAX - (int)Base::MAX;
		break;
	default: break;
	}

	switch ((SceneList)sceneNum)
	{
	case SceneList::TITLE:
		size += (int)Animation::Title::MAX - (int)Animation::Base::MAX;
		break;
	case SceneList::GAME:
		size += (int)Animation::Game::MAX - (int)Animation::Base::MAX;
		break;
	case SceneList::RESULT:
		size += (int)Animation::Result::MAX - (int)Animation::Base::MAX;
		break;
	default: break;
	}

	return size;
}

HRESULT Model::Load(int sceneNum)
{
	sceneNum_ = sceneNum;
	int size = 0;
	const string* fileName = nullptr;
	switch ((SceneList)sceneNum)
	{
	case SceneList::TITLE:
		size = (int)Title::MAX;
//		fileName = &titleFileName[0];
		break;
	case SceneList::GAME:
		size = (int)Game::MAX;
		fileName = &gameFileName[0];
		break;
	case SceneList::RESULT:
		size = (int)Result::MAX;
//		fileName = &resultFileName[0];
		break;
	default: break;
	}

	if (size > 0 && !fileName) { return E_FAIL; }
	for (int i = 0; i < size; ++i)
	{
		if (i < (int)Base::MAX) { continue; }
		HRESULT hr = systems_->GetGraphics()->GetWrapper()->LoadModel(fileName[i - (int)Base::MAX], i);
		if (FAILED(hr)) { return E_FAIL; }
		loading_->AddLoading();
	}

	// アニメーション
	size = 0;
	const ANIMATION_INFO* info = nullptr;
	switch ((SceneList)sceneNum)
	{
	case SceneList::TITLE:
		size = (int)Animation::Title::MAX;
		//		fileName = &titleFileName[0];
		break;
	case SceneList::GAME:
		size = (int)Animation::Game::MAX;
		info = &animationGameFileName[0];
		break;
	case SceneList::RESULT:
		size = (int)Animation::Result::MAX;
		//		fileName = &resultFileName[0];
		break;
	default: break;
	}

	if (size > 0 && !info) { return E_FAIL; }
	for (int i = 0; i < size; ++i)
	{
		if (i < (int)Animation::Base::MAX) { continue; }
		const auto& temp = info[i - (int)Animation::Base::MAX];
		HRESULT hr = systems_->GetGraphics()->GetWrapper()->LoadModelAnimation(temp.fileName, temp.parent);
		if (FAILED(hr)) { return E_FAIL; }
		loading_->AddLoading();
	}


	return S_OK;
}

void Model::Release(bool uninit)
{
	int size = 0;
	switch ((SceneList)sceneNum_)
	{
	case SceneList::TITLE:
		size = (int)Title::MAX;
		break;
	case SceneList::GAME:
		size = (int)Game::MAX;
		break;
	case SceneList::RESULT:
		size = (int)Result::MAX;
		break;
	default: break;
	}

	int max = (int)Base::MAX;
	if (uninit) { max = 0; }
	for (int i = size - 1; i >= max; --i)
	{
		systems_->GetGraphics()->GetWrapper()->ReleaseModel(i);
	}
}
