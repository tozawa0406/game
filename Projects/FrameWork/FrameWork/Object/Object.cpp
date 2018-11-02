//-----------------------------------------------------------------------------
//
//	�I�u�W�F�N�g[Object.cpp]
//	Auther : ���V�đ�
//																	2017/09/05
//-----------------------------------------------------------------------------
#include "Object.h"
#include "ObjectManager.h"
#include <algorithm>
#include "../Windows/Windows.h"
#include "../Scene/SceneManager.h"

#include "../Systems/Input/XInput.h"

//�R���X�g���N�^
Object::Object(Tag tag) : manager_(nullptr), tag_(tag), destroy_(false)
{
}

//�f�X�g���N�^
Object::~Object(void)
{
}

Controller* Object::GetCtrl(int i)
{
	if(manager_)
	{
		if (const auto& systems = manager_->GetSystems())
		{
			if (const auto& input = systems->GetInput())
			{
				if (const auto& ctrl = input->GetCtrl(i))
				{
					return ctrl;
				}
			}
		}
	}
	return nullptr;
}
