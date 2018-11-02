//-----------------------------------------------------------------------------
//
//	���U���g[ResultScene.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "ResultScene.h"
#include "../SceneManager.h"
#include "../../Windows/Windows.h"
#include "TitleScene.h"

// �R���X�g���N�^
ResultScene::ResultScene(SceneManager* manager) : BaseScene(manager),  GUI(manager->GetSystems(), nullptr, "SceneResult")
{
}

// �f�X�g���N�^
ResultScene::~ResultScene(void)
{
}

// �X�V����
int ResultScene::Update(void)
{
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneManager::CHANGE_SCENE;
	}

	return 0;
}
