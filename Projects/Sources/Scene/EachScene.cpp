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
		//�V�[���J��
		sceneNum = static_cast<SceneList>(static_cast<int>(sceneNum) + 1);

		//���U���g���z������^�C�g���ɖ߂�
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
		return static_cast<int>(SceneList::GAME);
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
	case SceneList::GAME:
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
	case SceneList::GAME:
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
