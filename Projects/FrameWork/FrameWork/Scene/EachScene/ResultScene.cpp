//-----------------------------------------------------------------------------
//
//	リザルト[ResultScene.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "ResultScene.h"
#include "../SceneManager.h"
#include "../../Windows/Windows.h"
#include "TitleScene.h"

// コンストラクタ
ResultScene::ResultScene(SceneManager* manager) : BaseScene(manager),  GUI(manager->GetSystems(), nullptr, "SceneResult")
{
}

// デストラクタ
ResultScene::~ResultScene(void)
{
}

// 更新処理
int ResultScene::Update(void)
{
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneManager::CHANGE_SCENE;
	}

	return 0;
}
