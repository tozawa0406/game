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
#include "../Object/StaticObject/WallParts.h"

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
	objectManager_ = new ObjectManager(this);
	assert(objectManager_);

	CreateField();

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
//	gameObject_[1] = objectManager_->Create<Dragon>();

	clearUI_ = objectManager_->Create<ClearFailed>();
	if (clearUI_)
	{
		clearUI_->SetEnable(false);
	}

	if(const auto& sound = GetSound())
	{
		sound->Play((int)Sound::Game::BGM_GAME);
	}
}

void GameScene::Uninit(void)
{
	if (const auto& sound = GetSound())
	{
		sound->Stop((int)Sound::Game::BGM_GAME);
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

void GameScene::CreateField(void)
{
	light_		= new Light(systems_);
	sky_		= new SkyDome(systems_);
	meshField_	= new MeshField(systems_);
	if (meshField_)
	{
		meshField_->Init(VECTOR2(50), VECTOR2(400));
	}

	objectManager_->Create<PaidGoodsBox>();
	objectManager_->Create<WallA>(VECTOR3( 100, 3,    0), VECTOR3(0,  1.57f, 0));
	objectManager_->Create<WallA>(VECTOR3(   0, 3, -100), VECTOR3(0,     0 , 0));
	objectManager_->Create<WallA>(VECTOR3(-100, 3,    0), VECTOR3(0, -1.57f, 0));
	objectManager_->Create<WallA>(VECTOR3(   0, 3,  100), VECTOR3(0,  3.14f, 0));

	// Žè‘O¶
	if (const auto& wallParts = objectManager_->Create<WallParts>())
	{
		wallParts->SetModelNum(Model::Game::ROCK_1);
		wallParts->SetTransform(Transform(VECTOR3(-80, 0, -80), VECTOR3(0), VECTOR3(2)));
		wallParts->SetColliderSize(VECTOR3(30, 30, 30));
	}

	// Žè‘O³–Ê
	if (const auto& wallParts = objectManager_->Create<WallParts>())
	{
		wallParts->SetModelNum(Model::Game::ROCK_14);
		wallParts->SetTransform(Transform(VECTOR3(0, -5, -90), VECTOR3(0.3f, 0, 0), VECTOR3(3)));
		wallParts->SetColliderOffset(VECTOR3(-8.5f, 15, 0), VECTOR3(-0.9f, 0, 0));
		wallParts->SetColliderSize(VECTOR3(45, 30, 25));
	}
}
