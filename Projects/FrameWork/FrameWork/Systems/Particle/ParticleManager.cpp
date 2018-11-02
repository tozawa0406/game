//-----------------------------------------------------------------------------
//
//	パーティクル管理[ParticleManager.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "ParticleManager.h"
#include "Particle.h"
#include "../GameSystems.h"
#include "../../Scene/SceneManager.h"
#include "../Camera/CameraManager.h"
#include <algorithm>

void ParticleManager::Uninit(void)
{
	for (UINT i = 0; i < obj_.size();)
	{
		RemoveVectorPtr(obj_, obj_[i]);
	}
}

//オブジェクトの更新処理
void ParticleManager::Update(void)
{
	for (auto& obj : obj_)
	{
		if (obj)
		{
			obj->Update();
		}
	}

	DestroyCheck();
}

void ParticleManager::Sort(void)
{
	int size = (int)obj_.size() - 1;

	std::vector<float> wVec;
	VECTOR3 wPos;
	VECTOR3 cameraPos;
	if (!systems_->GetSceneManager()) { return; }
	cameraPos = systems_->GetSceneManager()->GetCameraManager()->GetCamera()->GetPos();

	for (auto obj : obj_)
	{
		wPos = obj->GetTransform().position - cameraPos;
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

//オブジェクトの描画処理
void ParticleManager::Draw(void)
{
//	this->Sort();
	const auto& dev = systems_->GetRenderer()->GetWrapper();
	const auto& shader = systems_->GetShader()->GetShader(Shader::ENUM::POINT_SPRITE);
	for (auto& obj : obj_)
	{
		if (obj) 
		{
			shader->SetParam(MATRIX().Identity().Create(&obj->transform_), obj->data_.vertex.color, VECTOR4(0, 0, 1, 1));
			dev->Draw(obj, shader);
		}
	}
}

//オブジェクトの破壊
void ParticleManager::DestroyCheck(void)
{
	for (UINT i = 0; i < obj_.size();)
	{
		if (obj_[i]->destroy_)
		{
			RemoveVectorPtr(obj_, obj_[i]);
		}
		else
		{
			++i;
		}
	}
}
