//-----------------------------------------------------------------------------
//
//	�^�C�g��[TitleScene.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../Windows/Windows.h"

// �R���X�g���N�^
TitleScene::TitleScene(SceneManager* manager) : BaseScene(manager), GUI(manager->GetSystems(), nullptr, "SceneTitle")
{
}

// �f�X�g���N�^
TitleScene::~TitleScene(void)
{
}

// �X�V����
int TitleScene::Update(void)
{
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneManager::CHANGE_SCENE;
	}

	return 0;
}
