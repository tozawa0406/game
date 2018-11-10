//-----------------------------------------------------------------------------
//
//	���U���g[ResultScene.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "ResultScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include "TitleScene.h"

// �R���X�g���N�^
ResultScene::ResultScene(SceneManager* manager) : BaseScene(manager),  GUI(manager->GetSystems(), nullptr, "SceneResult")
{
}

// �f�X�g���N�^
ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
}

void ResultScene::Uninit(void)
{
}

// �X�V����
SceneList ResultScene::Update(void)
{
	if (GetCtrl(0)->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}
