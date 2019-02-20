#include "03_ButtleScene.h"
#include <FrameWork/Windows/Windows.h>

#include <FrameWork/Systems/Renderer/MeshField.h>
#include <FrameWork/Systems/Light.h>
#include <FrameWork/Systems/Score.h>

#include "../Object/GameObject/Player/Player.h"
#include "../Object/GameObject/Monster/Dragon/Dragon.h"
#include "../Object/UI/Timer.h"
#include "../Object/Wapon/KohakuSword.h"

#include "../Object/UI/ClearFailed.h"

#include "../Object/StaticObject/WallParts.h"
#include "../Object/UI/Tutorial/TutorialManager.h"

//! UI•\Ž¦‚µ‚Ä‚©‚çI—¹‚Ü‚Å‚ÌŽžŠÔ
static constexpr int END_TIME = 120;

ButtleScene::ButtleScene(void) : GUI(Systems::Instance(), nullptr, "SceneButtle")
	, objectManager_(nullptr)
	, clearUI_(nullptr)
	, endCnt_(0)
	, sky_(nullptr)
	, attackManager_(nullptr)
{
	for (auto& g : gameObject_) { g = nullptr; }
}

ButtleScene::~ButtleScene(void)
{
}

void ButtleScene::Init(SceneList sceneNum)
{
	BaseScene::Init(sceneNum);

	if (systems_)
	{
		if (const auto& graphics = systems_->GetGraphics())
		{
			if (const auto& renderTatget = graphics->GetRenderTarget())
			{
				if (const auto& cascadeManager = renderTatget->GetCascadeManager())
				{
					cascadeManager->SetFieldSize(1600);
				}
			}
		}
	}

	objectManager_ = new ObjectManager(this);
	assert(objectManager_);

	CreateField();

	attackManager_ = new AttackManager;

	auto tutorialManager = objectManager_->Create<TutorialManager>();
	auto player = objectManager_->Create<Player>(VECTOR3(0, 0, -170));
	assert(player);
	if (tutorialManager) 
	{
		tutorialManager->SetPlayer(player); 
		tutorialManager->SkipTutorial();
	}
	auto wapon  = objectManager_->Create<KohakuSword>();
	player->SetWapon(wapon);
	player->SetAttackManager(attackManager_);
	wapon->SetAttackManager(attackManager_);
	objectManager_->Create<Timer>();

#ifdef _DEBUG
	if (const auto& itemList = player->GetItemList())
	{
		itemList->AddItem(ItemID::Rations, -1);
		itemList->AddItem(ItemID::Recovery, 10);
	}
#endif

	auto* dragon = objectManager_->Create<Dragon>();
	if (dragon)
	{
		dragon->SetAttackManager(attackManager_);
	}

	gameObject_[0] = player;
	gameObject_[1] = dragon;

	clearUI_ = objectManager_->Create<ClearFailed>();
	if (clearUI_)
	{
		clearUI_->SetEnable(false);
	}
}

void ButtleScene::Uninit(void)
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

	if (systems_)
	{
		if (const auto& particleManager = systems_->GetParticleManager())
		{
			particleManager->Uninit();
		}
	}
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
		meshField_->Init(VECTOR2(40), VECTOR2(600), static_cast<int>(Resources::Texture::Buttle::FIELD));
	}

	// •Ç
	objectManager_->Create<WallParts>(VECTOR3( 200, 3, -100), VECTOR3(0,  1.57f, 0));
	objectManager_->Create<WallParts>(VECTOR3( 200, 3,  100), VECTOR3(0,  1.57f, 0));
	objectManager_->Create<WallParts>(VECTOR3(-100, 3, -200), VECTOR3(0,     0 , 0));
	objectManager_->Create<WallParts>(VECTOR3( 100, 3, -200), VECTOR3(0,     0 , 0));
	objectManager_->Create<WallParts>(VECTOR3(-200, 3, -100), VECTOR3(0, -1.57f, 0));
	objectManager_->Create<WallParts>(VECTOR3(-200, 3,  100), VECTOR3(0, -1.57f, 0));
	objectManager_->Create<WallParts>(VECTOR3(-100, 3,  200), VECTOR3(0,  3.14f, 0));
	objectManager_->Create<WallParts>(VECTOR3( 100, 3,  200), VECTOR3(0,  3.14f, 0));
}
