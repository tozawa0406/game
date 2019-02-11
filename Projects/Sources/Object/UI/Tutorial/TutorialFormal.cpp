#include "TutorialFormal.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Systems/Input/KeyInput.h>
#include "../../GameObject/Player/Player.h"

//! @def	描画順
static constexpr uint8	PRIORITY	= 210;
//! @def	アイコンサイズ
static constexpr float	UI_SIZE		= 40;
//! @def	文字サイズ
static constexpr float	FONT_SIZE	= UI_SIZE - 10;
//! @def	アイコンの表示位置X
static const VECTOR2 UI_ICON_POSITION	= VECTOR2((Windows::WIDTH * 0.5f) + 350, 50);
//! @def	アイコンテクスチャの分割数
static const VECTOR2 UI_ICON_SPLIT		= VECTOR2(6, 4);

TutorialFormal::TutorialFormal(void) : 
	player_(nullptr)
	, ctrlTypeIcon_(0)
{
}

TutorialFormal::~TutorialFormal(void)
{
}

void TutorialFormal::Init(TutorialManager* manager, Controller* ctrl)
{
	if (!manager) { return; }

	maxCnt_ = 0;
	TutorialBase::Init(manager, ctrl);

	manager->End();
	gauge_.SetEnable(false);
	gaugeBack_.SetEnable(false);

	int texNum = static_cast<int>(Resources::Texture::Camp::UI_TUTORIAL);
	float  icon[4] = { 0, 12, 18, 0 };
	string word[4] = { "抜刀", "回避", "抜刀", "走る" };

	for (int i = 0; i < 4; ++i)
	{
		auto& button = manual_[i].button;
		button.Init(PRIORITY, texNum);
		button.SetPosition(VECTOR2(UI_ICON_POSITION.x, UI_ICON_POSITION.y + (i * UI_SIZE)));
		button.SetSize(VECTOR2(UI_SIZE));
		button.SetSplit(UI_ICON_SPLIT);
		button.SetPattern(icon[i]);

		auto& name = manual_[i].name;
		name.Init(PRIORITY, "抜刀納刀緊急回避攻撃走る", static_cast<int>(FONT_SIZE));
		name.SetPosition(VECTOR2(UI_ICON_POSITION.x + Half(UI_SIZE), UI_ICON_POSITION.y - Half(FONT_SIZE) + (i * UI_SIZE)));
		name.SetSize(VECTOR2(FONT_SIZE));
		name.SetString(word[i]);
		name.SetColor(COLOR(0.5f, 0.5f, 0.5f));

		manual_[i].SetEnable(false);
	}

	player_ = manager->GetPlayer();
}

void TutorialFormal::Uninit(void)
{
	for (auto& manual : manual_)
	{
		manual.button.Uninit();
		manual.name.Uninit();
	}

	TutorialBase::Uninit();
}

TutorialBase* TutorialFormal::Update(void)
{
	if (!ctrl_ || !player_) { player_ = manager_->GetPlayer(); return nullptr; }

	JedgeCtrlType();

	Player::Animation anim = static_cast<Player::Animation>(player_->GetAnimation());
	switch (anim)
	{
	case Player::Animation::Wait:
	case Player::Animation::WaitTime1:
	case Player::Animation::WaitTime2:
		WaitState();
		break;
	case Player::Animation::Walk:
		WalkState();
		break;
	case Player::Animation::Run:
		RunState();
		break;
	case Player::Animation::Roll:
	case Player::Animation::Dive:
	case Player::Animation::KnockBack:
	case Player::Animation::KnockOut:
	case Player::Animation::Die:
	case Player::Animation::Earplug:
	case Player::Animation::Standup:
	case Player::Animation::Heal:
		NoIconState();
		break;
	case Player::Animation::WalkSetup:
		break;
	case Player::Animation::Setup:
		break;
	case Player::Animation::SetupDrawn:
		break;
	case Player::Animation::SetupWait:
		break;
	case Player::Animation::SetupWalk:
		break;
	case Player::Animation::Slash_1:
		break;
	case Player::Animation::Slash_2:
		break;
	case Player::Animation::Slash_3:
		break;
	case Player::Animation::MAX:
		break;
	default:
		break;
	}


	UpdateTimer();
	if (Finish()) { return nullptr; }

	return nullptr;
}

void TutorialFormal::JedgeCtrlType(void)
{
	if (!ctrl_) { return; }

	switch (ctrl_->GetCtrlNum())
	{
	case Controller::CtrlNum::Key:
		ctrlTypeIcon_ = 4;
		break;
	case Controller::CtrlNum::PS4:
		ctrlTypeIcon_ = 0;
		break;
	case Controller::CtrlNum::X:
		ctrlTypeIcon_ = 2;
		break;
	}
}

void TutorialFormal::SetIcon(int num, float* pattern, string* word)
{
	if (!pattern || !word) { return; }

	for (int i = 0; i < num; ++i)
	{
		manual_[i].SetEnable(true);
		auto& button = manual_[i].button;
		button.SetPosition(VECTOR2(UI_ICON_POSITION.x, UI_ICON_POSITION.y + (i * UI_SIZE)));
		button.SetPattern(pattern[i]);

		auto& name = manual_[i].name;
		name.SetPosition(VECTOR2(UI_ICON_POSITION.x + Half(UI_SIZE), UI_ICON_POSITION.y - Half(FONT_SIZE) + (i * UI_SIZE)));
		name.SetString(word[i]);
	}
}

void TutorialFormal::NoIconState(void)
{
	for (auto& manual : manual_)
	{
		manual.SetEnable(false);
	}
}

void TutorialFormal::WaitState(void)
{
	float  icon[2] = { 0 + ctrlTypeIcon_, 12 + ctrlTypeIcon_ };
	string word[2] = { "抜刀", "回避" };

	SetIcon(2, icon, word);
	manual_[2].SetEnable(false);
	manual_[3].SetEnable(false);
}

void TutorialFormal::WalkState(void)
{
	float  icon[3] = { 0 + ctrlTypeIcon_, 12 + ctrlTypeIcon_, 13 + ctrlTypeIcon_ };
	string word[3] = { "攻撃", "回避", "走る" };

	SetIcon(3, icon, word);
	manual_[3].SetEnable(false);
}

void TutorialFormal::RunState(void)
{
	float  icon[2] = { 0 + ctrlTypeIcon_, 12 + ctrlTypeIcon_ };
	string word[2] = { "攻撃", "緊急回避" };

	SetIcon(2, icon, word);
	manual_[2].SetEnable(false);
	manual_[3].SetEnable(false);
}
