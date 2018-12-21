#include "ObjectManager.h"
#include "Object.h"
#include <algorithm>
#include "../Scene/SceneManager.h"

ObjectManager::ObjectManager(BaseScene* scene) : BaseManager(Systems::Instance()), scene_(scene)
{
}

ObjectManager::~ObjectManager(void)
{
}

HRESULT ObjectManager::Init(void)
{
	return S_OK;
}

void ObjectManager::Uninit(void)
{
	for (auto& obj : obj_) { obj->Uninit(); }

	for (uint i = 0; i < obj_.size();)
	{
		RemoveVectorPtr(obj_, obj_[i]);
	}
}

void ObjectManager::Update(void)
{
	uint size = (uint)obj_.size();
	for (uint i = 0; i < size; ++i)
	{
		if (obj_[i] != nullptr)
		{
			obj_[i]->Update();
		}
	}

	DestroyCheck();
}

void ObjectManager::DestroyCheck(void)
{
	for (uint i = 0; i < obj_.size();)
	{
		if (obj_[i]->GetDestroy())
		{
			obj_[i]->Uninit();
			RemoveVectorPtr(obj_, obj_[i]);
		}
		else
		{
			i++;
		}
	}
}
