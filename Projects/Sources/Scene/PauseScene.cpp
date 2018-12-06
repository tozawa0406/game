#include "PauseScene.h"
#include <FrameWork/Scene/SceneManager.h>

// 定数定義
static constexpr int TITLE_CONTROLLER		= 20000;
static constexpr int PAUSE_SELECT_UP		= 0;
static constexpr int PADDING_TIME			= 30;

/* @brief	コンストラクタ			*/
Pause::Pause(SceneManager* manager) : BaseScene(manager),  GUI(manager->GetSystems(), nullptr, "ScenePause")
	 , selectNum_(0)
	 , paddingTimeCnt_(0)
{
	 for (auto& p : pos_) { p = VECTOR2(0, 0); }
}

/* @brief	デストラクタ			*/
Pause::~Pause(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	なし					*/
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

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void Pause::Uninit(void)
{
}

/* @brief	更新処理
 * @param	なし
 * @return	シーン遷移フラグ		*/
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
		if (selectNum_ < SELECT_NUM - 1)
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
	for (int i = 0; i < SELECT_NUM; ++i)
	{
		if (i != selectNum_)
		{
			notSelect_[not].position = menu_[i].position;
			not++;
		}
	}

	return SceneList::NOTCHANGE;
}

/* @brief	UIの使用状態を一括変更処理
 * @param	(enable)	使用状態
 * @return	なし					*/
void Pause::EnableUI(bool enable)
{
	back_.enable = enable;
	for (int i = 0; i < SELECT_NUM; ++i)
	{
		menu_[i].enable = enable; 
		menuBack_[i].enable = enable;
		if (i < SELECT_NUM - 1)
		{
			notSelect_[i].enable = enable;
		}
	}
}
