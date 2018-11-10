//-----------------------------------------------------------------------------
//
//	ポーズ[PauseScene.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "PauseScene.h"
#include <FrameWork/Scene/SceneManager.h>

// コンストラクタ
 Pause::Pause(SceneManager* manager) : BaseScene(manager),  GUI(manager->GetSystems(), nullptr, "ScenePause")
	 , selectNum_(0)
	 , paddingTimeCnt_(0)
{
}

 // デストラクタ
 Pause::~Pause(void)
 {
 }

 void Pause::Init(void)
 {
	 // 暗転
	 back_.position = VECTOR2((float)Half(Graphics::WIDTH), (float)Half(Graphics::HEIGHT));
	 back_.size = VECTOR2((float)Graphics::WIDTH, (float)Graphics::HEIGHT);
	 back_.color = COLOR::RGBA(0, 0, 0, 100);
	 back_.Init(systems_, 250, (int)Texture::Base::FILL_RECTANGLE);

	 // メニュー
	 for (int i = 0; i < 3; ++i)
	 {
		 // メニュー背景
		 menuBack_[i].color = COLOR(1, 1, 1, 1);
		 menuBack_[i].position = VECTOR2((float)Half(Graphics::WIDTH), PAUSE_H * (1.0f + (2 * i)) + Half(PAUSE_H));
		 menuBack_[i].size = VECTOR2(PAUSE_W, PAUSE_H);
		 menuBack_[i].Init(systems_, 252, (int)Texture::Base::FILL_RECTANGLE);

		 // 
		 menu_[i].position = menuBack_[i].position;
		 menu_[i].color = COLOR::RGBA(100, 100, 100, 255);
		 menu_[i].size = menuBack_[i].size;
		 menu_[i].pattern = (float)i;
		 menu_[i].split.y = 3;
		 //		menu_[i].Init(systems_, 253, (int)Texture::Game::MENU);

		 if (i < 2)
		 {
			 // 非選択オブジェクトを暗くするため
			 notSelect_[i].Init(systems_, 254, (int)Texture::Base::FILL_RECTANGLE);
			 notSelect_[i].position = VECTOR2((float)Half(Graphics::WIDTH), PAUSE_H * (1.0f + (2 * (i + 1))) + Half(PAUSE_H));
			 notSelect_[i].size = menu_[i].size;
			 notSelect_[i].color = COLOR::RGBA(0, 0, 0, 150);
		 }
	 }
	 EnableUI(false);
 }

 void Pause::Uninit(void)
 {
 }

 // 更新処理
 SceneList Pause::Update(void)
{
	EnableUI(true);

	// メニュー決定後の時間
	if (paddingTimeCnt_ > PADDING_TIME)
	{
		// シーン遷移
		paddingTimeCnt_ = 0;
		if (selectNum_ == 0) { EnableUI(false); }
		return static_cast<SceneList>(selectNum_ + 1);
	}
	// 選択してから遷移までの時間
	else if (paddingTimeCnt_ > 0) 
	{
		paddingTimeCnt_++;

		// 点滅処理
		if (paddingTimeCnt_ % 10 < 5)
		{
			menuBack_[selectNum_].color = COLOR(1, 1, 1, 1);
		}
		else
		{
			menuBack_[selectNum_].color = COLOR::RGBA(100, 100, 100, 200);
		}

		return SceneList::NOTCHANGE;
	}

	//上下キー入力
	if (GetCtrl(0)->Trigger(Input::AXIS_LY, DIK_S, true) < 0 || GetCtrl(0)->Trigger(Input::GAMEPAD_DOWN))
	{
		if (selectNum_ < PAUSE_SELECT_DOWN)
		{
//			systems_->GetSound()->Play((int)Sound::Base::SE_SELECT);
			selectNum_++;
		}
	}
	else if (GetCtrl(0)->Trigger(Input::AXIS_LY, DIK_W) > 0 || GetCtrl(0)->Trigger(Input::GAMEPAD_UP))
	{
		if (selectNum_ > PAUSE_SELECT_UP)
		{
//			systems_->GetSound()->Play((int)Sound::Base::SE_SELECT);
			selectNum_--;
		}
		
	}

	//決定キー
	if (GetCtrl(0)->Press(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
//		systems_->GetSound()->Play((int)Sound::Base::SE_ENTER);
		paddingTimeCnt_++;
	}

	if (GetCtrl(0)->Trigger(Input::GAMEPAD_START, DIK_P))
	{
//		systems_->GetSound()->Play((int)Sound::Base::SE_SELECT);
		selectNum_ = 0;
		EnableUI(false);
		manager_->SetPause(false);
		return SceneList::NOTCHANGE;
	}

	int not = 0;
	// 非選択オブジェクトの選定
	for (int i = 0; i < 3; ++i)
	{
		if (i != selectNum_)
		{
			notSelect_[not].position = menu_[i].position;
			not++;
		}
	}

	return SceneList::NOTCHANGE;
}

// UIの使用を一気に変更
void Pause::EnableUI(bool enable)
{
	back_.enable = enable;
	for (int i = 0; i < 3; ++i)
	{
		menu_[i].enable = enable; 
		menuBack_[i].enable = enable;
		if (i < 2)
		{
			notSelect_[i].enable = enable;
		}
	}
}
