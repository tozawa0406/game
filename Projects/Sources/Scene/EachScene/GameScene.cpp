//-----------------------------------------------------------------------------
//
//	�Q�[��[GameScene.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "GameScene.h"
#include <FrameWork/Windows/Windows.h>

#include <FrameWork/Object/ObjectManager.h>

#include <FrameWork/Systems/Renderer/MeshField.h>
#include <FrameWork/Systems/Light.h>
#include <FrameWork/Systems/Score.h>

#include "../../Object/Player/PlayerHunter.h"
#include "../../Object/Monster/Dragon.h"
#include "../../Object/UI/Timer.h"
#include "../../Object/StaticObject/PaidGoodsBox.h"
#include "../../Object/Wapon/KohakuSword.h"

// �R���X�g���N�^
GameScene::GameScene(SceneManager* manager) : BaseScene(manager), GUI(manager->GetSystems(), nullptr, "SceneGame"), objectManager_(nullptr)
{
	light_ = new Light(systems_);
	sky_   = new SkyDome(systems_);
	meshField_ = new MeshField(systems_);
	meshField_->Init(50, 200);

	objectManager_ = new ObjectManager(this);

	auto* player = objectManager_->Create<PlayerHunter>();
	auto* wapon  = objectManager_->Create<KohakuSword>();
	player->SetWapon(wapon);
	objectManager_->Create<Dragon>();
	objectManager_->Create<Timer>();
	objectManager_->Create<PaidGoodsBox>();

//	systems_->GetSound()->Play((int)Sound::Game::BGM_GAME);
}

// �f�X�g���N�^
GameScene::~GameScene(void)
{
	systems_->GetSound()->Stop((int)Sound::Game::BGM_GAME);
	DeletePtr(meshField_);
	DeletePtr(sky_);
	UninitDeletePtr(objectManager_);
	DeletePtr(light_);
}

// �X�V����
int GameScene::Update(void)
{
	//�t���[�����J�E���g
	frameCnt++;

	if (sky_)
	{
		sky_->Update();
	}
	//�I�u�W�F�N�g�X�V
	objectManager_->Update();

	if (systems_->GetInput()->GetCtrl(0)->Trigger(Input::GAMEPAD_START, DIK_P)) 
	{
		systems_->GetSound()->Play((int)Sound::Base::SE_SELECT);
		manager_->SetPause(true);
	}

	return 0;
}
