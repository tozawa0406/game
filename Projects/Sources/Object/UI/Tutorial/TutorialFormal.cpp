#include "TutorialFormal.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Systems/Input/KeyInput.h>
#include "../../GameObject/Player/Player.h"

TutorialFormal::TutorialFormal(void) : 
	player_(nullptr)
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

	player_ = manager->GetPlayer();
}

void TutorialFormal::Uninit(void)
{
	TutorialBase::Uninit();
}

TutorialBase* TutorialFormal::Update(void)
{
	if (!ctrl_ || !player_) { player_ = manager_->GetPlayer(); return nullptr; }

	JedgeCtrlType();

	UpdateTimer();
	if (Finish()) { return nullptr; }

	return nullptr;
}

void TutorialFormal::JedgeCtrlType(void)
{
	if (!ctrl_) { return; }
}
