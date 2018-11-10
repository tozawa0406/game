//-----------------------------------------------------------------------------
//
//	�^�C�g��[TitleScene.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>

// �R���X�g���N�^
TitleScene::TitleScene(SceneManager* manager) : BaseScene(manager), GUI(manager->GetSystems(), nullptr, "SceneTitle")
{
}

// �f�X�g���N�^
TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
}

void TitleScene::Uninit(void)
{
}

// �X�V����
SceneList TitleScene::Update(void)
{
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}
