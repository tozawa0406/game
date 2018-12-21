//-----------------------------------------------------------------------------
//
//	オブジェクト管理[ObjectManager.h]
//	Auther : 戸澤翔太
//																	2017/09/05
//-----------------------------------------------------------------------------
#include "ObjectManager.h"
#include "Object.h"
#include <algorithm>
#include "../Scene/SceneManager.h"

#include "../Systems/Camera/CameraManager.h"
#include "../Systems/Camera/MoveCamera.h"
//#include "Player.h"

ObjectManager::ObjectManager(BaseScene* scene) : BaseManager(Systems::Instance()), scene_(scene)
{
}

void ObjectManager::Uninit(void)
{
	for (auto& obj : obj_) { obj->Uninit(); }

	for (uint i = 0; i < obj_.size();)
	{
		RemoveVectorPtr(obj_, obj_[i]);
	}
}

//オブジェクトの更新処理
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

//オブジェクトの破壊
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
