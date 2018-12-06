#include "ResultScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include "TitleScene.h"

/* @brief	�R���X�g���N�^			*/
ResultScene::ResultScene(SceneManager* manager) : BaseScene(manager),  GUI(manager->GetSystems(), nullptr, "SceneResult")
{
}

/* @brief	�f�X�g���N�^			*/
ResultScene::~ResultScene(void)
{
}

/* @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void ResultScene::Init(void)
{
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void ResultScene::Uninit(void)
{
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	�V�[���D���t���O		*/
SceneList ResultScene::Update(void)
{
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}
