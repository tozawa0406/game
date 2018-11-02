//-----------------------------------------------------------------------------
//
//	オブジェクト[Object.cpp]
//	Auther : 戸澤翔太
//																	2017/09/05
//-----------------------------------------------------------------------------
#include "Object.h"
#include "ObjectManager.h"
#include <algorithm>
#include "../Windows/Windows.h"
#include "../Scene/SceneManager.h"

#include "../Systems/Input/XInput.h"

//コンストラクタ
Object::Object(Tag tag) : manager_(nullptr), tag_(tag), destroy_(false)
{
}

//デストラクタ
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
