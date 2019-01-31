#include "01_TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>

//! @def	UI描画準
static constexpr int UI_PRIORITY = 200;
//! @def	UI文字サイズ
static constexpr int UI_TEXT_SIZE = 40;
//! @def	タイトルサイズ
static constexpr int UI_TITLE_SIZE = 100;
//! @def	タイトル位置
static const VECTOR2 TITLE_POSITION = VECTOR2(Half(Windows::WIDTH) - 125, Half(Windows::HEIGHT) - 100);
//! @def	press描画の位置
static const VECTOR2 PRESS_POSITION = VECTOR2(Half(Windows::WIDTH) - 165, Windows::HEIGHT * 0.9f - UI_TEXT_SIZE);
//! @def	pree描画のサイズ
static const VECTOR2 PRESS_SIZE = VECTOR2(Quarter(Windows::WIDTH), Half(Windows::HEIGHT * 0.1f));
//! @def	UIボタンの描画位置調整
static constexpr float ADJUST_POSITION_X = 6 * UI_TEXT_SIZE + UI_TEXT_SIZE * 0.5f;

//! @def	点滅間隔
static constexpr float FLASHING_RANGE = 60;

TitleScene::TitleScene(void) : GUI(Systems::Instance(), nullptr, "SceneTitle")
	, frameCnt_(0)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	// タイトル
	title_.Init(UI_PRIORITY, "TITLE", UI_TITLE_SIZE);
	title_.SetPosition(TITLE_POSITION);
	title_.SetColor(COLOR(0, 0, 0));

	// 「please press」
	press_.Init(UI_PRIORITY, "Please Press", UI_TEXT_SIZE);
	press_.SetPosition(PRESS_POSITION);
	press_.SetColor(COLOR(0, 0, 0, 1));

	// 各種ボタン
	button_.Init(UI_PRIORITY, "Enter 〇 B", UI_TEXT_SIZE);
	button_.SetPosition(VECTOR2(PRESS_POSITION.x + ADJUST_POSITION_X, PRESS_POSITION.y));
	button_.SetColor(COLOR(0, 0, 0));

	// 背景
	back_.Init(UI_PRIORITY - 1, static_cast<int>(Resources::Texture::Base::WHITE));
	back_.SetPosition(VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT)));
	back_.SetSize(VECTOR2(Windows::WIDTH, Windows::HEIGHT));
	back_.SetColor(COLOR::RGBA(150, 150, 150));

	if (const auto& sound = GetSound())
	{
		sound->Play((int)Resources::Sound::Title::BGM_TITLE);
	}

	if (const auto& load = manager_->GetDontDestroyOnLoad())
	{
		load->Reset();
	}
}

void TitleScene::Uninit(void)
{
	if (const auto& sound = GetSound())
	{
		sound->Stop((int)Resources::Sound::Title::BGM_TITLE);
	}
	title_.Uninit();
	back_.Uninit();
	button_.Uninit();
	press_.Uninit();
}

SceneList TitleScene::Update(void)
{
	// コントローラの取得
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	// 入力判定
	JudgeCtrlType(*ctrl);
	// 点滅
	Flashing();

	// 遷移処理
	return EndScene(*ctrl);
}

void TitleScene::Flashing(void)
{
	// フレームカウンタ
	frameCnt_++;
	if (frameCnt_ > FLASHING_RANGE * 2) { frameCnt_ = 0; }
	
	// αの設定
	float a = (frameCnt_ < FLASHING_RANGE) ? frameCnt_ / FLASHING_RANGE : 2 - frameCnt_ / FLASHING_RANGE;
	auto c = press_.GetColor();
	c.a = a;
	press_.SetColor(c);
	c = button_.GetColor();
	c.a = a;
	button_.SetColor(c);
}

void TitleScene::JudgeCtrlType(Controller& ctrl)
{
	switch (static_cast<Controller::CtrlNum>(ctrl.GetCtrlNum()))
	{
	case Controller::CtrlNum::Key:
		button_.SetString("Enter");
		break;
	case Controller::CtrlNum::PS4:
		button_.SetString("〇");
		break;
	case Controller::CtrlNum::X:
		button_.SetString("B");
		break;
	}
}

SceneList TitleScene::EndScene(Controller& ctrl)
{
	// 入力
	if (ctrl.Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{		
		if (const auto& sound = GetSound())
		{
			sound->Play(static_cast<int>(Resources::Sound::Base::SE_ENTER));
		}
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}

void TitleScene::GuiUpdate(void)
{
	ImGui::Text("a : %.2f", (frameCnt_ < FLASHING_RANGE) ? frameCnt_ / FLASHING_RANGE : 2 - frameCnt_ / FLASHING_RANGE);
}
