#include "03_ButtleScene.h"
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

#include "../Object/StaticObject/Wall.h"
#include "../Object/StaticObject/PaidGoodsBox.h"
#include "../Object/StaticObject/Cart.h"
#include "../Object/StaticObject/Cannon.h"
#include "../Object/StaticObject/Tent.h"

//! UI•\Ž¦‚µ‚Ä‚©‚çI—¹‚Ü‚Å‚ÌŽžŠÔ
static constexpr int END_TIME = 120;

ButtleScene::ButtleScene(void) : GUI(Systems::Instance(), nullptr, "SceneGame")
	, objectManager_(nullptr)
	, clearUI_(nullptr)
	, endCnt_(0)
	, sky_(nullptr)
{
	for (auto& g : gameObject_) { g = nullptr; }
}

ButtleScene::~ButtleScene(void)
{
}

void ButtleScene::Init(void)
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
		sound->Play((int)Resources::Sound::Camp::BGM_GAME);
	}
}

void ButtleScene::Uninit(void)
{
	if (const auto& sound = GetSound())
	{
		sound->Stop((int)Resources::Sound::Camp::BGM_GAME);
	}
	UninitDeletePtr(objectManager_);
	DeletePtr(meshField_);
	DeletePtr(sky_);
	DeletePtr(light_);
}

SceneList ButtleScene::Update(void)
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
		systems_->GetSound()->Play((int)Resources::Sound::Base::SE_SELECT);
		manager_->SetPause(true);
	}

	return SceneList::NOTCHANGE;
}

void ButtleScene::CreateField(void)
{
	light_		= new Light(systems_);
	meshField_  = new MeshField(systems_);
	sky_		= new SkyDome(systems_);
	if (meshField_)
	{
		meshField_->Init(VECTOR2(50), VECTOR2(400), static_cast<int>(Texture::Camp::FIELD));
	}

	objectManager_->Create<Wall>();
	objectManager_->Create<PaidGoodsBox>();
	objectManager_->Create<Cart>();
	objectManager_->Create<Tent>();
	objectManager_->Create<Cannon>();
}
