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

#include "../Object/UI/ClearFailed.h"

#include "../Object/StaticObject/PaidGoodsBox.h"
#include "../Object/StaticObject/WallA.h"

//! UI•\Ž¦‚µ‚Ä‚©‚çI—¹‚Ü‚Å‚ÌŽžŠÔ
static constexpr int END_TIME = 120;

GameScene::GameScene(void) : GUI(Systems::Instance(), nullptr, "SceneGame")
	, objectManager_(nullptr)
	, clearUI_(nullptr)
	, endCnt_(0)
	, sky_(nullptr)
	, meshField_(nullptr)
	, light_(nullptr)
{
	for (auto& g : gameObject_) { g = nullptr; }
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	light_		= new Light(systems_);
	sky_		= new SkyDome(systems_);
	meshField_	= new MeshField(systems_);
	if (meshField_)
	{
		meshField_->Init(VECTOR2(50), VECTOR2(400));
	}

	objectManager_ = new ObjectManager(this);
	assert(objectManager_);
	objectManager_->Create<PaidGoodsBox>();
	objectManager_->Create<WallA>(VECTOR3( 100, 3,    0), VECTOR3(0,  1.57f, 0));
	objectManager_->Create<WallA>(VECTOR3(   0, 3, -100), VECTOR3(0,      0, 0));
	objectManager_->Create<WallA>(VECTOR3(-100, 3,    0), VECTOR3(0, -1.57f, 0));
	objectManager_->Create<WallA>(VECTOR3(   0, 3,  100), VECTOR3(0,  3.14f, 0));

	auto* player = objectManager_->Create<Player>();
	assert(player);
	auto* wapon  = objectManager_->Create<KohakuSword>();
	player->SetWapon(wapon);
	objectManager_->Create<Timer>();
	if (auto* life = objectManager_->Create<PlayerLife>())
	{
		life->SetPlayer(player);
	}
	if (auto* item = objectManager_->Create<ItemList>())
	{
		item->SetPlayer(player);
	}

	gameObject_[0] = player;
	gameObject_[1] = objectManager_->Create<Dragon>();

	if (clearUI_ = objectManager_->Create<ClearFailed>())
	{
		clearUI_->SetEnable(false);
	}

	if (systems_)
	{
		systems_->GetSound()->Play((int)Sound::Game::BGM_GAME);
	}
}

void GameScene::Uninit(void)
{
	if (systems_)
	{
		systems_->GetSound()->Stop((int)Sound::Game::BGM_GAME);
	}
	UninitDeletePtr(objectManager_);
	DeletePtr(meshField_);
	DeletePtr(sky_);
	DeletePtr(light_);
}

SceneList GameScene::Update(void)
{
	if (sky_) { sky_->Update(); }
	if (objectManager_) { objectManager_->Update(); }
	if (light_) { light_->Update(); }

	// ”s–k
	if (gameObject_[0] && gameObject_[0]->IsDed())
	{
		clearUI_->SetColor(COLOR(0, 0, 1, 1));
		clearUI_->SetPattern(1);
		clearUI_->SetEnable(true);
		endCnt_++;
	}
	// Ÿ—˜
	else if (gameObject_[1] && gameObject_[1]->IsDed())
	{
		clearUI_->SetColor(COLOR(1, 0, 0, 1));
		clearUI_->SetPattern(0);
		clearUI_->SetEnable(true);
		endCnt_++;
	}

	if (endCnt_ > END_TIME)
	{
		return SceneList::NEXT;
	}


	if (systems_->GetInput()->GetCtrl(0)->Trigger(Input::GAMEPAD_START, DIK_P)) 
	{
		systems_->GetSound()->Play((int)Sound::Base::SE_SELECT);
		manager_->SetPause(true);
	}

	return SceneList::NOTCHANGE;
}
