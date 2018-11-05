//-----------------------------------------------------------------------------
//
//	シーン処理[SceneManager.cpp]
//	Auther : 戸澤翔太
//																	2017/09/05
//-----------------------------------------------------------------------------
#include "SceneManager.h"
#include "../Windows/Windows.h"
#include "../Systems/GameSystems.h"

#include "EachScene/TitleScene.h"
#include "../../../Sources/Scene/EachScene/GameScene.h"
#include "EachScene/ResultScene.h"
#include "EachScene/PauseScene.h"

#include "../Systems/Input/KeyInput.h"
#include "../Systems/Input/Controller.h"
#include "../Systems/Camera/CameraManager.h"

// コンストラクタ
SceneManager::SceneManager(Systems* systems) : Interface(systems), sceneChange_(CHANGE_SCENE), displayMode_(Scene::Num::MAX)
											 , scene_(nullptr), pause_(nullptr), fadeCnt_(SCENE_FADE_OUT - 1), isPause_(false), startLoad_(false)
{
	camera_  = new CameraManager(this);

#ifdef _DEBUG
	displayMode_ = Scene::Num::TITLE;
#endif
	fade_.Init(systems_, 250, (int)Texture::Base::FILL_RECTANGLE);
	fade_.position = { (float)Half(Graphics::WIDTH), (float)Half(Graphics::HEIGHT) };
	fade_.size     = { (float)Graphics::WIDTH, (float)Graphics::HEIGHT };
	fade_.color    = COLOR(0, 0, 0, 0);
	fade_.enable = false;

	loading_ = new Loading(systems_);
	loading_->Init();
}

// デストラクタ
SceneManager::~SceneManager(void)
{
	loading_->Uninit();
	DeletePtr(loading_);
	DeletePtr(pause_);
	DeletePtr(scene_);
	DeletePtr(camera_);
}

// 更新処理
void SceneManager::Update(void)
{	
	// シーンの更新
	SceneUpdate();

	// 実際のシーン遷移処理
	ChangeActual();

	// 強制シーン遷移
	ForceSceneChange();
}

// 描画処理
void SceneManager::Draw(void)
{
	camera_->Draw();

	// フェード処理
	Fade();
}

// シーンの更新処理
void SceneManager::SceneUpdate(void)
{	
	if (!isPause_)
	{
		camera_->Update();
		if (!GetSystems()->GetDebug()->GetDebugPause())
		{
			if (scene_)
			{
				Change(scene_->Update());
			}
		}
	}
	else
	{
		if (pause_)
		{
			switch (pause_->Update())
			{
			case 1:
				isPause_ = false;
				break;
			case 2:
				Change(CHANGE_GAME);
				DeletePtr(pause_);
				break;
			case 3:
				Change(CHANGE_TITLE);
				DeletePtr(pause_);
				break;
			}
		}
	}
}

// フェード処理
void SceneManager::Fade(void)
{
	int a = 0;
	if (fade_.enable)
	{
		fade_.enable = false;
		loading_->EndFade();
	}
	if (fadeCnt_ < SCENE_FADE_OUT)
	{
		//秒数から 1フレーム毎のαを設定
		a = (255 / SCENE_FADE_OUT) * fadeCnt_;
	}
	else
	{
		//秒数から 1フレーム毎のαを設定
		a = 255 - (255 / SCENE_FADE_IN) * (fadeCnt_ - SCENE_FADE_OUT);
	}
	//シーン切り替え時の暗転
	if (sceneChange_ & CHANGE_CHECK)
	{
		COLOR c = { 0, 0, 0, a / 255.0f };
		fade_.color = c;
		fade_.enable = true;
		loading_->FadeAlpha(c.a);
	}
}

// 外部呼出し遷移準備処理
void SceneManager::Change(int scene)
{
	//シーン遷移フラグをtrue
	if (!sceneChange_)
	{
		sceneChange_ = scene;
	}
}

// 実際にシーンが切り替わる処理
void SceneManager::ChangeActual(void)
{
	//外部呼出しされたら
	if (sceneChange_)
	{
		//フェードの進行
		fadeCnt_++;
		//フェードの終了時間を越えたら
		if (fadeCnt_ == SCENE_FADE_OUT)
		{
			if (!startLoad_)
			{
				isPause_ = false;			//ポーズ状態を解除

				if (sceneChange_ & CHANGE_SCENE)
				{
					//シーン遷移
					displayMode_ = (Scene::Num)((int)displayMode_ + 1);

					if (displayMode_ > Scene::Num::RESULT)
					{//リザルトを越えたらタイトルに戻る
						displayMode_ = Scene::Num::TITLE;
					}
				}
				else if (sceneChange_ & CHANGE_TITLE)
				{
					displayMode_ = Scene::Num::TITLE;
				}
				else if (sceneChange_ & CHANGE_GAME)
				{
					displayMode_ = Scene::Num::GAME;
				}

				DeletePtr(scene_);
				DeletePtr(pause_);
				loading_->Start((int)displayMode_);
				startLoad_ = true;
			}
			if (!loading_->IsLoading())
			{
				loading_->End();
				startLoad_ = false;
				scene_ = this->CreateScene();
			}
			if (startLoad_)
			{
				fadeCnt_--;
			}
		}
		else if (fadeCnt_ > SCENE_FADE_OUT + SCENE_FADE_IN)
		{
			//フェード終了
			sceneChange_ = CHANGE_NORMAL;
			fadeCnt_ = 0;
		}
	}
}

// BACKボタンで遷移
void SceneManager::ForceSceneChange(void)
{
#ifdef _SELF_DEBUG
	if(systems_->GetInput()->GetCtrl(0)->Press(Input::GAMEPAD_HOME, DIK_BACK))
	{
		Change(); 
	}
#endif
}

BaseScene* SceneManager::CreateScene(void)
{
	switch (displayMode_)
	{
	case Scene::Num::TITLE:
		return new TitleScene(this);
	case Scene::Num::GAME:
		pause_ = new Pause(this);
		return new GameScene(this);
	case Scene::Num::RESULT:
		return new ResultScene(this);
	}
	return nullptr;
}
