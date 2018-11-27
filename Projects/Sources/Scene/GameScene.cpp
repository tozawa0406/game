//-----------------------------------------------------------------------------
//
//	ゲーム[GameScene.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "GameScene.h"
#include <FrameWork/Windows/Windows.h>

#include <FrameWork/Systems/Renderer/MeshField.h>
#include <FrameWork/Systems/Light.h>
#include <FrameWork/Systems/Score.h>

#include "../Object/GameObject/Player/Player.h"
#include "../Object/UI/PlayerLife.h"
#include "../Object/UI/ItemList.h"
#include "../Object/GameObject/Monster/Dragon/Dragon.h"
#include "../Object/UI/Timer.h"
#include "../Object/Wapon/KohakuSword.h"

#include "../Object/StaticObject/PaidGoodsBox.h"
#include "../Object/StaticObject/WallA.h"

// コンストラクタ
GameScene::GameScene(SceneManager* manager) : BaseScene(manager), GUI(manager->GetSystems(), nullptr, "SceneGame"), objectManager_(nullptr)
{
}

// デストラクタ
GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	light_ = new Light(systems_);
	sky_ = new SkyDome(systems_);
	meshField_ = new MeshField(systems_);
	meshField_->Init(VECTOR2(50), VECTOR2(400));

	objectManager_ = new ObjectManager(this);

	objectManager_->Create<PaidGoodsBox>();
	objectManager_->Create<WallA>(VECTOR3( 100, 3,    0), VECTOR3(0,  1.57f, 0));
	objectManager_->Create<WallA>(VECTOR3(   0, 3, -100), VECTOR3(0,      0, 0));
	objectManager_->Create<WallA>(VECTOR3(-100, 3,    0), VECTOR3(0, -1.57f, 0));
	objectManager_->Create<WallA>(VECTOR3(   0, 3,  100), VECTOR3(0,  3.14f, 0));

	auto* player = objectManager_->Create<Player>();
	auto* wapon = objectManager_->Create<KohakuSword>();
	player->SetWapon(wapon);
	objectManager_->Create<Dragon>();
	objectManager_->Create<Timer>();
	auto* life = objectManager_->Create<PlayerLife>();
	life->SetPlayer(player);
	auto* item = objectManager_->Create<ItemList>();
	item->SetPlayer(player);

	//	systems_->GetSound()->Play((int)Sound::Game::BGM_GAME);
}

void GameScene::Uninit(void)
{
	systems_->GetSound()->Stop((int)Sound::Game::BGM_GAME);
	DeletePtr(meshField_);
	DeletePtr(sky_);
	UninitDeletePtr(objectManager_);
	DeletePtr(light_);
}

// 更新処理
SceneList GameScene::Update(void)
{
	//フレームをカウント
	frameCnt++;

	if (sky_)
	{
		sky_->Update();
	}
	//オブジェクト更新
	objectManager_->Update();

	if (systems_->GetInput()->GetCtrl(0)->Trigger(Input::GAMEPAD_START, DIK_P)) 
	{
		systems_->GetSound()->Play((int)Sound::Base::SE_SELECT);
		manager_->SetPause(true);
	}

	return SceneList::NOTCHANGE;
}
