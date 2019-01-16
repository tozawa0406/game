#include "EachScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "TitleScene.h"
#include "GameScene.h"
#include "PauseScene.h"
#include "ResultScene.h"

EachScene::EachScene(void)
{
}

EachScene::~EachScene(void)
{
}

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

int EachScene::ChangePause(int returnPause)
{
	switch (returnPause)
	{
	case 1:
		return 0;
	case 2:
		return static_cast<int>(SceneList::CAMP);
	case 3:
		return static_cast<int>(SceneList::TITLE);
	}

	return -1;
}

BaseScene* EachScene::CreateScene(SceneManager* manager, SceneList sceneNum)
{
	BaseScene* temp = nullptr;

	switch (sceneNum)
	{
	case SceneList::TITLE:
		temp = new TitleScene;
		break;
	case SceneList::CAMP:
		temp = new GameScene;
		break;
	case SceneList::BUTTLE:
		temp = new GameScene;
		break;
	case SceneList::RESULT:
		temp = new ResultScene;
		break;
	default:
		break;
	}

	if (temp) 
	{
		temp->SetManager(manager);
		temp->Init(); 
	}

	return temp;
}

BaseScene* EachScene::CreatePause(SceneManager* manager, SceneList sceneNum)
{
	BaseScene* temp = nullptr;

	switch (sceneNum)
	{
	case SceneList::CAMP:
	case SceneList::BUTTLE:
		temp = new Pause;
		break;
	default:
		break;
	}

	if (temp) 
	{
		temp->SetManager(manager);
		temp->Init(); 
	}

	return temp;
}
