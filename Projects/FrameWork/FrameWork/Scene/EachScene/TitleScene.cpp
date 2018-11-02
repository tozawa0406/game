//-----------------------------------------------------------------------------
//
//	タイトル[TitleScene.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../Windows/Windows.h"

// コンストラクタ
TitleScene::TitleScene(SceneManager* manager) : BaseScene(manager), GUI(manager->GetSystems(), nullptr, "SceneTitle")
{
}

// デストラクタ
TitleScene::~TitleScene(void)
{
}

// 更新処理
int TitleScene::Update(void)
{
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneManager::CHANGE_SCENE;
	}

	return 0;
}
