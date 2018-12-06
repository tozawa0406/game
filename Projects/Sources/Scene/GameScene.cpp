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

/* @brief	コンストラクタ			*/
GameScene::GameScene(SceneManager* manager) : BaseScene(manager), GUI(manager->GetSystems(), nullptr, "SceneGame")
	, objectManager_(nullptr)
	, sky_(nullptr)
	, meshField_(nullptr)
	, light_(nullptr)
{
}

/* @brief	デストラクタ			*/
GameScene::~GameScene(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	なし					*/
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
	auto* wapon  = objectManager_->Create<KohakuSword>();
	player->SetWapon(wapon);
	objectManager_->Create<Dragon>();
	objectManager_->Create<Timer>();
	auto* life = objectManager_->Create<PlayerLife>();
	life->SetPlayer(player);
	auto* item = objectManager_->Create<ItemList>();
	item->SetPlayer(player);

	//	systems_->GetSound()->Play((int)Sound::Game::BGM_GAME);
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void GameScene::Uninit(void)
{
	systems_->GetSound()->Stop((int)Sound::Game::BGM_GAME);

	UninitDeletePtr(objectManager_);
	DeletePtr(meshField_);
	DeletePtr(sky_);
	DeletePtr(light_);
}

/* @brief	更新処理
 * @param	なし
 * @return	シーン遷移番号			*/
SceneList GameScene::Update(void)
{
	if (sky_) { sky_->Update(); }
	if (objectManager_) { objectManager_->Update(); }

	if (systems_->GetInput()->GetCtrl(0)->Trigger(Input::GAMEPAD_START, DIK_P)) 
	{
		systems_->GetSound()->Play((int)Sound::Base::SE_SELECT);
		manager_->SetPause(true);
	}

	return SceneList::NOTCHANGE;
}
