#include "ResultScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include "TitleScene.h"

/* @brief	コンストラクタ			*/
ResultScene::ResultScene(SceneManager* manager) : BaseScene(manager),  GUI(manager->GetSystems(), nullptr, "SceneResult")
{
}

/* @brief	デストラクタ			*/
ResultScene::~ResultScene(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	なし					*/
void ResultScene::Init(void)
{
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void ResultScene::Uninit(void)
{
}

/* @brief	更新処理
 * @param	なし
 * @return	シーン船員フラグ		*/
SceneList ResultScene::Update(void)
{
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}
