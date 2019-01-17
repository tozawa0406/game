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
	if (!systems_) { return E_FAIL; }

	int size = 0, max = 0;
	const string* fileName = systems_->GetResource().LoadTexture(SceneList::MAX, size, max);

	if (const auto& graphics = systems_->GetGraphics())
	{
		if (const auto& wrapper = graphics->GetWrapper())
		{
			for (int i = 0; i < max; ++i)
			{
				HRESULT hr = wrapper->LoadTexture(fileName[i], i);
				if (FAILED(hr)) { return E_FAIL; }
			}
		}
		else { return E_FAIL; }
	}
	else { return E_FAIL; }

	return S_OK;
}

void Texture::Uninit(void)
{
	Release(true);
}

int Texture::SetUpLoading(Loading* loading, int sceneNum)
{
	loading_ = loading;

	if (!systems_) { return 0; }
	int size = 0, max = 0;
	systems_->GetResource().LoadTexture(static_cast<SceneList>(sceneNum), size, max);

	return size;
}

HRESULT Texture::Load(int sceneNum)
{
	sceneNum_ = sceneNum;

	if (!systems_) { return E_FAIL; }
	int size = 0, max = 0;
	const auto& fileName = systems_->GetResource().LoadTexture(static_cast<SceneList>(sceneNum), size, max);

	if (const auto& graphics = systems_->GetGraphics())
	{
		if (const auto& wrapper = graphics->GetWrapper())
		{
			for (int i = 0; i < max; ++i)
			{
				if (i < static_cast<int>(Resources::Texture::Base::MAX)) { continue; }
				HRESULT hr = wrapper->LoadTexture(fileName[i - static_cast<int>(Resources::Texture::Base::MAX)], i);
				if (FAILED(hr)) { 
					return E_FAIL; }
				loading_->AddLoading();
			}
		}
		else { return E_FAIL; }
	}
	else { return E_FAIL; }

	return S_OK;
}

void Texture::Release(bool uninit)
{
	if (!systems_) { return; }
	int size = 0, max = 0;
	systems_->GetResource().LoadTexture(static_cast<SceneList>(sceneNum_), size, max);

	int baseMax = static_cast<int>(Resources::Texture::Base::MAX);
	if (uninit) { baseMax = 0; }

	if (const auto& graphics = systems_->GetGraphics())
	{
		if (const auto& wrapper = graphics->GetWrapper())
		{
			for (int i = max - 1; i >= baseMax; --i)
			{
				wrapper->ReleaseTexture(i);
			}
		}
	}
}

VECTOR2 Texture::GetTextureSize(int texNum) const
{
	return systems_->GetGraphics()->GetWrapper()->GetTextureSize(texNum);
}
