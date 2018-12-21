//-----------------------------------------------------------------------------
//
//	�I�u�W�F�N�g�Ǘ�[ObjectManager.h]
//	Auther : ���V�đ�
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

//�I�u�W�F�N�g�̍X�V����
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

//�I�u�W�F�N�g�̔j��
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
