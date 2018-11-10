//-----------------------------------------------------------------------------
//
//	リザルト[ResultScene.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "ResultScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include "TitleScene.h"

// コンストラクタ
ResultScene::ResultScene(SceneManager* manager) : BaseScene(manager),  GUI(manager->GetSystems(), nullptr, "SceneResult")
{
}

// デストラクタ
ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
}

void ResultScene::Uninit(void)
{
}

// 更新処理
SceneList ResultScene::Update(void)
{
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}
