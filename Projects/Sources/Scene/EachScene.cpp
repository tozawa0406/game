#include "EachScene.h"
#include <FrameWork/Scene/SceneManager.h>

#include "TitleScene.h"
#include "GameScene.h"
#include "PauseScene.h"
#include "ResultScene.h"

/* @brief	�R���X�g���N�^			*/
EachScene::EachScene(void)
{
}

/* @brief	�f�X�g���N�^			*/
EachScene::~EachScene(void)
{
}

/* @brief	�V�[���Ǘ��̕ϐ��̒l��ύX����
 * @param	(sceneChange)	�V�[���J�ڃt���O
 * @param	(sceneNum)		�V�[���Ǘ��ԍ�
 * @return	�Ȃ�					*/
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

/* @brief	�|�[�Y�I�����̃R�}���h
 * @param	(returnPause)		�|�[�Y�̕Ԃ�l
 * @return	���̃V�[���Ɉڂ�ꍇ��0�ȊO				*/
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

/* @brief	�V�[����new����
 * @param	(sceneNum)		�V�[���Ǘ��ԍ�
 * @return	�V�[���ԍ��ɉ������V�[����new����		*/
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
 * @brief	�|�[�Y�V�[����new����
 * @param	(sceneNum)		�V�[���Ǘ��ԍ�
 * @return	�|�[�Y��new�����A�g��Ȃ��ꍇ��nullptr		*/
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
