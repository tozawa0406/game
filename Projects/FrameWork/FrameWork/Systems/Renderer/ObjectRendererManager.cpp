//-----------------------------------------------------------------------------
//
//	3D•`‰æŠÇ—[ObjectRendererManager.cpp]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "ObjectRendererManager.h"

#include "ObjectRenderer.h"
#include "Sprite/SpriteRenderer.h"
#include "Model/MeshRenderer.h"
#include "../../Windows/Windows.h"
#include "../GameSystems.h"
#include "../../Scene/SceneManager.h"
#include "../Camera/CameraManager.h"

#include "../../Graphics/DirectX11/DirectX11.h"
#include "Shader/Default.h"
#include "Shader/CascadeShadow.h"

void ObjectRendererManager::Add(ObjectRenderer* obj)
{
	BaseManager::Add(obj);
	this->Sort();
}

void ObjectRendererManager::Sort(void)
{
	int size = (int)obj_.size() - 1;

	std::vector<float> wVec;
	VECTOR3 wPos;
	VECTOR3 cameraPos;
	if (!systems_->GetSceneManager()) { return; }
	cameraPos = systems_->GetSceneManager()->GetCameraManager()->GetCamera()->GetPos();

	for (auto obj : obj_)
	{
		wPos = obj->GetTransform()->position - cameraPos;
		float vecTemp;
		vecTemp = VecLengthSq(wPos);
		wVec.emplace_back(vecTemp);
	}

	bool sort = false;
	for (int j = 0; j < size; j++)
	{
		sort = false;
		for (int i = 0; i < size; i++)
		{
			if (!obj_[i]->sort) { continue; }
			if (wVec[i] < wVec[i + 1])
			{
				Swap(obj_[i], obj_[i + 1]);
				Swap(wVec[i], wVec[i + 1]);
				sort = true;
			}
		}
		if (!sort) { break; }
	}
}

void ObjectRendererManager::FastDraw(void)
{
	const auto& directX11	= static_cast<DirectX11*>(systems_->GetRenderer());
	const auto& target		= directX11->GetRenderTarget();
	const auto& dev			= systems_->GetRenderer()->GetWrapper();

	target->BeginMultiRendererTarget();
	dev->BeginDrawObjectRenderer();

	for (auto& obj : obj_)
	{
		if (obj->enable && obj->fastDraw)
		{
			if (obj->type == ObjectRenderer::RendererType::SPRITE)
			{
				Shader* shader = nullptr;
				const auto& sprite = (SpriteRenderer*)obj;
				if (obj->shader != Shader::ENUM::UNKOUWN)
				{
					shader = systems_->GetShader()->GetShader(obj->shader);
					shader->SetParam(MATRIX().Identity().Create(obj->transform_), obj->material.diffuse, sprite->GetTexcoord());
				}
				dev->Draw(sprite, shader);
			}
			else if (obj->type == ObjectRenderer::RendererType::MODEL)
			{
				Shader* shader = nullptr;
				const auto& model = (MeshRenderer*)obj;
				if (obj->shader != Shader::ENUM::UNKOUWN)
				{
					shader = systems_->GetShader()->GetShader(obj->shader);
					shader->SetParam(MATRIX().Identity().Create(obj->transform_), obj->material.diffuse, VECTOR4(0, 0, 1, 1));
				}
				dev->Draw(model, shader);
			}
		}
	}

	dev->EndDrawObjectRenderer();
}

void ObjectRendererManager::Draw(void)
{	
	const auto& directX11	= static_cast<DirectX11*>(systems_->GetRenderer());
	const auto& target		= directX11->GetRenderTarget();
	const auto& dev			= systems_->GetRenderer()->GetWrapper();

	this->Sort();
	dev->BeginDrawObjectRenderer();

	for (auto& obj : obj_)
	{
		if (obj->enable && !obj->fastDraw)
		{
			if (obj->type == ObjectRenderer::RendererType::SPRITE)
			{
				Shader* shader = nullptr;
				const auto& sprite = (SpriteRenderer*)obj;
				if (obj->shader != Shader::ENUM::UNKOUWN)
				{
					shader = systems_->GetShader()->GetShader(obj->shader);
					shader->SetParam(MATRIX().Identity().Create(obj->transform_), obj->material.diffuse, sprite->GetTexcoord());
				}
				dev->Draw(sprite, shader);
			}
			else if (obj->type == ObjectRenderer::RendererType::MODEL)
			{
				Shader* shader = nullptr;
				const auto& model = (MeshRenderer*)obj;
				if (obj->shader != Shader::ENUM::UNKOUWN)
				{
					shader = systems_->GetShader()->GetShader(obj->shader);
					shader->SetParam(MATRIX().Identity().Create(obj->transform_), obj->material.diffuse, VECTOR4(0, 0, 1, 1));
				}
				dev->Draw(model, shader);
			}
		}
	}

	dev->EndDrawObjectRenderer();
	target->EndMultiRendererTarget();
}

void ObjectRendererManager::DrawShadow(void)
{
	const auto& graphics = systems_->GetRenderer();
	const auto& dev = graphics->GetWrapper();

	CascadeShadow* shader = static_cast<CascadeShadow*>(systems_->GetShader()->GetShader(Shader::ENUM::ZTEXTURE));
	const auto& skinning = systems_->GetShader()->GetShader(Shader::ENUM::SKINNING_SHADOW);
	dev->BeginDrawObjectRenderer();

	for (int i = 0; i < MAX_CASCADE; ++i)
	{
		shader->BeginDraw(i);

		for (auto& obj : obj_)
		{
			if (obj->shadow)
			{
				if (obj->type == ObjectRenderer::RendererType::SPRITE)
				{
					const auto& sprite = (SpriteRenderer*)obj;
					dev->Draw(sprite, shader);
				}
				else if (obj->type == ObjectRenderer::RendererType::MODEL)
				{
					const auto& model = (MeshRenderer*)obj;
					if (model->shader == Shader::ENUM::DEFAULT)
					{
						dev->Draw(model, shader);
					}
					else
					{
						dev->Draw(model, shader);
					}
				}
			}
		}
		shader->EndDraw();
	}

	dev->EndDrawObjectRenderer();
}
