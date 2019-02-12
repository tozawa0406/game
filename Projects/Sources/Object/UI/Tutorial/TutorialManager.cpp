#include "TutorialManager.h"
#include <FrameWork/Windows/Windows.h>
#include "Tutorial01Move.h"
#include "TutorialFormal.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Object/ObjectManager.h>

const VECTOR2 TutorialManager::POSITION = VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT) - 150);
const VECTOR2 TutorialManager::POSITION_KEYBOARD = VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT) - 175);
const VECTOR2 TutorialManager::SIZE_KEY = VECTOR2(512, 217) * 0.6f;
const VECTOR2 TutorialManager::SIZE_PS4 = VECTOR2(512, 320) * 0.6f;
const VECTOR2 TutorialManager::SIZE_X   = VECTOR2(470, 320) * 0.6f;

//! @def	ƒRƒ“ƒgƒ[ƒ‰”Ô†
static constexpr int CTRL_NUM = 0;

TutorialManager::TutorialManager(void) : Object(ObjectTag::UI)
	, tutorial_(nullptr)
	, player_(nullptr)
	, end_(false)
{
}

TutorialManager::~TutorialManager(void)
{
}

void TutorialManager::Init(void)
{
	if (!manager_) { return; }

	if (const auto& scene = manager_->GetScene())
	{
		if (scene->GetSceneNum() == SceneList::CAMP)
		{
			ctrlImage_.Init(210, static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL));
			ctrlImage_.SetPosition(POSITION);
			ctrlImage_.SetSize(VECTOR2(512, 320) * 0.6f);
			ctrlImage_.SetColor(COLOR(1, 1, 1, 0.75f));

			ctrlStick_[0].Init(211, static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_L3));
			ctrlStick_[1].Init(211, static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_R3));

			charactor_.Init(210, static_cast<int>(Resources::Texture::Base::LOAD));
			charactor_.SetSplit(VECTOR2(3, 2));
			charactor_.SetPosition(VECTOR2(Windows::WIDTH - 100, 150));
			charactor_.SetSize(VECTOR2(95, 60));
			charactor_.SetPattern(2);

			baloon_.Init(210, static_cast<int>(Resources::Texture::Camp::UI_BALOON));
			baloon_.SetPosition(VECTOR2(Windows::WIDTH - 270, 160));
			baloon_.SetSize(VECTOR2(250, 60));
		}
	}

	for (auto& stick : ctrlStick_)
	{
		stick.SetPosition(ctrlImage_.GetPosition());
		stick.SetSize(ctrlImage_.GetSize());
		stick.SetColor(ctrlImage_.GetColor());
	}
}

void TutorialManager::Uninit(void)
{
	baloon_.Uninit();
	charactor_.Uninit();
	UninitDeletePtr(tutorial_);
	for (auto& stick : ctrlStick_) { stick.Uninit(); }
	ctrlImage_.Uninit();
}

void TutorialManager::Update(void)
{
	JudgeCtrlType();

	if (tutorial_)
	{
		const auto& next = tutorial_->Update();
		if (next)
		{
			UninitDeletePtr(tutorial_);
			tutorial_ = next;
			tutorial_->Init(this, GetCtrl(CTRL_NUM));
		}
	}
}

void TutorialManager::JudgeCtrlType(void)
{
	if (!manager_) { return; }
	if (const auto& scene = manager_->GetScene())
	{
		if (scene->GetSceneNum() != SceneList::CAMP) { return; }
	}

	uint8 ctrlNum = 0;
	if (const auto& ctrl = GetCtrl(CTRL_NUM))
	{
		ctrlNum = ctrl->GetCtrlNum();
	}

	VECTOR2 pos   = 0;
	VECTOR2 size  = 0;
	int texNum[3] = { 0, 0, 0 };
	switch (ctrlNum)
	{
	case Controller::CtrlNum::Key:
		pos			= POSITION_KEYBOARD;
		size		= SIZE_KEY;
		texNum[0]	= static_cast<int>(Resources::Texture::Camp::UI_KEY);
		for (auto& stick : ctrlStick_) { stick.SetEnable(false); }
		break;
	case Controller::CtrlNum::PS4:
		pos			= POSITION;
		size		= SIZE_PS4;
		texNum[0]	= static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL);
		texNum[1]	= static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_L3);
		texNum[2]	= static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_R3);
		for (auto& stick : ctrlStick_) { stick.SetEnable(!end_); }
		break;
	case Controller::CtrlNum::X:
		pos			= POSITION;
		size		= SIZE_X;
		texNum[0]	= static_cast<int>(Resources::Texture::Camp::UI_XCTRL);
		texNum[1]	= static_cast<int>(Resources::Texture::Camp::UI_XCTRL_L);
		texNum[2]	= static_cast<int>(Resources::Texture::Camp::UI_XCTRL_R);
		for (auto& stick : ctrlStick_) { stick.SetEnable(!end_); }
		break;
	}
	CanvasRenderer::Image* img[3] = { &ctrlImage_, &ctrlStick_[0], &ctrlStick_[1] };

	for (int i = 0; i < 3; ++i)
	{
		img[i]->SetTexNum(texNum[i]);
		img[i]->SetSize(size);
		img[i]->SetPosition(pos);
	}
	ctrlImage_.SetEnable(!end_);
}

void TutorialManager::SkipTutorial(void)
{
	UninitDeletePtr(tutorial_);
	tutorial_ = new TutorialFormal;
	if (tutorial_)
	{
		tutorial_->Init(this, GetCtrl(CTRL_NUM));
	}
}

void TutorialManager::StartTutorial(void)
{
	UninitDeletePtr(tutorial_);
	tutorial_ = new Tutorial01Move;
	if (tutorial_)
	{
		tutorial_->Init(this, GetCtrl(CTRL_NUM));
	}
}
