#include "02_CampScene.h"
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

#include "../Object/StaticObject/CampWall.h"
#include "../Object/StaticObject/PaidGoodsBox.h"
#include "../Object/StaticObject/Cart.h"
#include "../Object/StaticObject/Cannon.h"
#include "../Object/StaticObject/Tent.h"

CampScene::CampScene(void) : GUI(Systems::Instance(), nullptr, "SceneCamp")
	, objectManager_(nullptr)
	, sky_(nullptr)
	, goal_(nullptr)
	, attackManager_(nullptr)
{
}

CampScene::~CampScene(void)
{
}

void CampScene::Init(void)
{
	if (systems_)
	{
		if (const auto& graphics = systems_->GetGraphics())
		{
			if (const auto& renderTatget = graphics->GetRenderTarget())
			{
				if (const auto& cascadeManager = renderTatget->GetCascadeManager())
				{
					cascadeManager->SetFieldSize(600);
				}
			}
		}
	}

	objectManager_ = new ObjectManager(this);
	assert(objectManager_);

	CreateField();

	attackManager_ = new AttackManager;

	auto* player = objectManager_->Create<Player>(VECTOR3(0));
	assert(player);
	auto* wapon  = objectManager_->Create<KohakuSword>();
	player->SetWapon(wapon);
	player->SetAttackManager(attackManager_);
	wapon->SetAttackManager(attackManager_);
	objectManager_->Create<Timer>();

	if(const auto& sound = GetSound())
	{
		sound->Play((int)Resources::Sound::Camp::BGM_GAME);
	}
}

void CampScene::Uninit(void)
{
	if (const auto& sound = GetSound())
	{
		sound->Stop((int)Resources::Sound::Camp::BGM_GAME);
	}
	UninitDeletePtr(attackManager_);
	UninitDeletePtr(objectManager_);
	DeletePtr(meshField_);
	DeletePtr(sky_);
	DeletePtr(light_);
}

SceneList CampScene::Update(void)
{
	if (sky_) { sky_->Update(); }
	if (objectManager_) { objectManager_->Update(); }
	if (light_) { light_->Update(); }

	if (systems_->GetInput()->GetCtrl(0)->Trigger(Input::GAMEPAD_START, DIK_P)) 
	{
		systems_->GetSound()->Play((int)Resources::Sound::Base::SE_SELECT);
		manager_->SetPause(true);
	}

	if (goal_ && goal_->IsChange()) { return SceneList::NEXT; }

	return SceneList::NOTCHANGE;
}

void CampScene::CreateField(void)
{
	light_		= new Light(systems_);
	meshField_  = new MeshField(systems_);
	sky_		= new SkyDome(systems_);
	if (meshField_)
	{
		meshField_->Init(VECTOR2(50), VECTOR2(400), static_cast<int>(Resources::Texture::Camp::FIELD));
	}

	objectManager_->Create<CampWall>();
	objectManager_->Create<PaidGoodsBox>();
	objectManager_->Create<Cart>();
	objectManager_->Create<Tent>();
	objectManager_->Create<Cannon>();
	objectManager_->Create<Goal>();

	goal_ = objectManager_->Create<Goal>();
	assert(goal_);
}
