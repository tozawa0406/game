#include "TutorialManager.h"
#include <FrameWork/Windows/Windows.h>
#include "Tutorial01Move.h"
#include <FrameWork/Systems/Input/Controller.h>

const VECTOR2 TutorialManager::POSITION = VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT) - 150);

TutorialManager::TutorialManager(void) : Object(ObjectTag::UI)
	, tutorial_(nullptr)
{
}

TutorialManager::~TutorialManager(void)
{
}

void TutorialManager::Init(void)
{
	ctrlImage_.Init(210, static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL));
	ctrlImage_.SetPosition(POSITION);
	ctrlImage_.SetSize(VECTOR2(512, 320) * 0.6f);
	ctrlImage_.SetColor(COLOR(1, 1, 1, 0.75f));

	ctrlStick_[0].Init(211, static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_L3));
	ctrlStick_[1].Init(211, static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_R3));

	for (auto& stick : ctrlStick_)
	{
		stick.SetPosition(ctrlImage_.GetPosition());
		stick.SetSize(ctrlImage_.GetSize());
		stick.SetColor(ctrlImage_.GetColor());
	}

	tutorial_ = new Tutorial01Move;
	if (tutorial_)
	{
		tutorial_->Init(this, GetCtrl(0));
	}
}

void TutorialManager::Uninit(void)
{
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
			tutorial_->Uninit();
			tutorial_ = next;
			tutorial_->Init(this, GetCtrl(0));
		}
	}
}

void TutorialManager::JudgeCtrlType(void)
{
	if (!manager_) { return; }
	uint8 ctrlNum = 0;
	if (const auto& ctrl = GetCtrl(0))
	{
		ctrlNum = ctrl->GetCtrlNum();
	}

	VECTOR2 size = 0;
	int texNum[3] = { 0, 0, 0 };
	switch (ctrlNum)
	{
	case Controller::CtrlNum::Key:
		size		= VECTOR2(512, 320) * 0.6f;
		texNum[0]	= static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL);
		texNum[1]	= static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_L3);
		texNum[2]	= static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_R3);
		break;
	case Controller::CtrlNum::PS4:
		size		= VECTOR2(512, 320) * 0.6f;
		texNum[0]	= static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL);
		texNum[1]	= static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_L3);
		texNum[2]	= static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_R3);
		break;
	case Controller::CtrlNum::X:
		size		= VECTOR2(470, 320) * 0.6f;
		texNum[0]	= static_cast<int>(Resources::Texture::Camp::UI_XCTRL);
		texNum[1]	= static_cast<int>(Resources::Texture::Camp::UI_XCTRL_L);
		texNum[2]	= static_cast<int>(Resources::Texture::Camp::UI_XCTRL_R);
		break;
	}
	CanvasRenderer::Image* img[3] = { &ctrlImage_, &ctrlStick_[0], &ctrlStick_[1] };

	for (int i = 0;i < 3;++i)
	{
		img[i]->SetTexNum(texNum[i]);
		img[i]->SetSize(size);
	}
}
