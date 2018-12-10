//-----------------------------------------------------------------------------
//
//	テクスチャ管理[Texture.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Texture.h"
#include "../../../Windows/Windows.h"
#include "../../GameSystems.h"
#include "../../../Scene/BaseScene.h"
#include "../../Loading.h"

Texture::Texture(Systems* systems) : Interface(systems), sceneNum_(-1)
{
}

HRESULT Texture::Init(void)
{
	int size = (int)Texture::Base::MAX;
	for (int i = 0;i < size; ++i)
	{
		HRESULT hr = systems_->GetRenderer()->GetWrapper()->LoadTexture(baseFileName[i], i);
		if (FAILED(hr)) { return E_FAIL; }
	}
	return S_OK;
}

void Texture::Uninit(void)
{
	Release(true);
}

int Texture::SetUpLoading(Loading* loading, int sceneNum)
{
	loading_ = loading;

	switch ((SceneList)sceneNum)
	{
	case SceneList::TITLE:
		return (int)Title::MAX - (int)Base::MAX;
	case SceneList::GAME:
		return (int)Game::MAX - (int)Base::MAX;
	case SceneList::RESULT:
		return (int)Result::MAX - (int)Base::MAX;
	default: break;
	}
	return 0;
}

HRESULT Texture::Load(int sceneNum)
{
	sceneNum_ = sceneNum;
	int size = 0;
	const string* fileName = nullptr;
	switch ((SceneList)sceneNum)
	{
	case SceneList::TITLE:
		size = (int)Title::MAX;
		fileName = &titleFileName[0];
		break;
	case SceneList::GAME:
		size = (int)Game::MAX;
		fileName = &gameFileName[0];
		break;
	case SceneList::RESULT:
		size = (int)Result::MAX;
	//	fileName = &resultFileName[0];
		break;
	default: break;
	}

	if (!fileName && size != (int)Base::MAX) { return E_FAIL; }
	for (int i = 0; i < size; ++i)
	{
		if (i < (int)Base::MAX) { continue; }
		HRESULT hr = systems_->GetRenderer()->GetWrapper()->LoadTexture(fileName[i - (int)Base::MAX], i);
		if (FAILED(hr)) { return E_FAIL; }
		loading_->AddLoading();
	}

	return S_OK;
}

void Texture::Release(bool uninit)
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
		systems_->GetRenderer()->GetWrapper()->ReleaseTexture(i);
	}
}

VECTOR2 Texture::GetTextureSize(int texNum) const
{
	return systems_->GetRenderer()->GetWrapper()->GetTextureSize(texNum);
}
