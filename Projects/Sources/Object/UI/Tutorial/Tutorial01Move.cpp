#include "Tutorial01Move.h"
#include <FrameWork/Systems/Input/Controller.h>

Tutorial01Move::Tutorial01Move(void) :
	stick_(nullptr)
	, defaultPosition(VECTOR2(0))
{
}

Tutorial01Move::~Tutorial01Move(void)
{
}

void Tutorial01Move::Init(TutorialManager* manager, Controller* ctrl)
{
	if (!manager) { return; }
	TutorialBase::Init(manager, ctrl);	

	stick_ = manager_->GetStickUIPtr(false);
	if (stick_)
	{
		defaultPosition = stick_->GetPosition();
		stick_->SetColor(COLOR(1, 0.5f, 0.5f, 1));
	}

	maxCnt_ = 5 * 60;
}

void Tutorial01Move::Uninit(void)
{
	stick_->SetColor(COLOR(1, 1, 1, 0.75f));
}

TutorialBase* Tutorial01Move::Update(void)
{
	if (!ctrl_ || !stick_) { return nullptr; }

	stick_->SetColor(COLOR(1, 0.5f, 0.5f, 1));
	// ¶\ŽšƒL[‚Ì‘€ì
	SHORT x = ctrl_->GetAxis().stickLX;
	SHORT y = ctrl_->GetAxis().stickLY;
	if (x != 0 || y != 0)
	{
		VECTOR2 pos = defaultPosition;
		pos.x += x * 0.1f;
		pos.y += y * 0.1f;
		stick_->SetPosition(pos);
		stick_->SetColor(COLOR(1, 0, 0, 1));
		cnt_++;
	}

	if (Finish()) { return nullptr; }

	return nullptr;
}
