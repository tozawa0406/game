#include "EachScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "TitleScene.h"
#include "GameScene.h"
#include "PauseScene.h"
#include "ResultScene.h"

/* @brief	コンストラクタ			*/
EachScene::EachScene(void)
{
}

/* @brief	デストラクタ			*/
EachScene::~EachScene(void)
{
}

/* @brief	シーン管理の変数の値を変更する
 * @param	(sceneChange)	シーン遷移フラグ
 * @param	(sceneNum)		シーン管理番号
 * @return	なし					*/
void EachScene::ChangeScene(SceneList sceneChange, SceneList& sceneNum)
{
	if (sceneChange == SceneList::NEXT)
	{
		//シーン遷移
		sceneNum = static_cast<SceneList>(static_cast<int>(sceneNum) + 1);

		//リザルトを越えたらタイトルに戻る
		if (sceneNum >= SceneList::MAX)
		{
			sceneNum = SceneList::TITLE;
		}
	}
	else
	{
		sceneNum = sceneChange;
	}
}

/* @brief	ポーズ終了時のコマンド
 * @param	(returnPause)		ポーズの返り値
 * @return	他のシーンに移る場合は0以外				*/
int EachScene::ChangePause(int returnPause)
{
	switch (returnPause)
	{
	case 1:
		return 0;
	case 2:
		return static_cast<int>(SceneList::GAME);
	case 3:
		return static_cast<int>(SceneList::TITLE);
	}

	return -1;
}

/* @brief	シーンのnew処理
 * @param	(sceneNum)		シーン管理番号
 * @return	シーン番号に応じたシーンのnew処理		*/
BaseScene* EachScene::CreateScene(SceneManager* manager, SceneList sceneNum)
{
	BaseScene* temp = nullptr;

	switch (sceneNum)
	{
	case SceneList::TITLE:
		temp = new TitleScene(manager);
		break;
	case SceneList::GAME:
		temp = new GameScene(manager);
		break;
	case SceneList::RESULT:
		temp = new ResultScene(manager);
		break;
	default:
		break;
	}

	if (temp) { temp->Init(); }

	return temp;
}

/* @fn		CreatePause
 * @brief	ポーズシーンのnew処理
 * @param	(sceneNum)		シーン管理番号
 * @return	ポーズのnew処理、使わない場合はnullptr		*/
BaseScene* EachScene::CreatePause(SceneManager* manager, SceneList sceneNum)
{
	BaseScene* temp = nullptr;

	switch (sceneNum)
	{
	case SceneList::GAME:
		temp = new Pause(manager);
		break;
	default:
		break;
	}

	if (temp) { temp->Init(); }

	return temp;
}
