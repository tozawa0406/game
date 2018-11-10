//-----------------------------------------------------------------------------
//
//	タイトル[TitleScene.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>

// コンストラクタ
TitleScene::TitleScene(SceneManager* manager) : BaseScene(manager), GUI(manager->GetSystems(), nullptr, "SceneTitle")
{
}

// デストラクタ
TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
}

void TitleScene::Uninit(void)
{
}

// 更新処理
SceneList TitleScene::Update(void)
{
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}
