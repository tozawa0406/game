#include "Tutorial01Move.h"
#include <FrameWork/Systems/Input/Controller.h>

#include "Tutorial02Camera.h"

static constexpr short THRESHOLD = 10;

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

	maxCnt_ = 5 * 60;
	TutorialBase::Init(manager, ctrl);	

	stick_ = manager_->GetStickUIPtr(false);
	if (stick_)
	{
		defaultPosition = stick_->GetPosition();
		stick_->SetColor(COLOR(1, 0.5f, 0.5f, 1));
	}
}

void Tutorial01Move::Uninit(void)
{
	stick_->SetPosition(defaultPosition);
	stick_->SetColor(COLOR(1, 1, 1, 0.75f));

	TutorialBase::Uninit();
}

TutorialBase* Tutorial01Move::Update(void)
{
	if (!ctrl_ || !stick_) { return nullptr; }

	// デフォルトの設定
	stick_->SetPosition(defaultPosition);
	stick_->SetColor(COLOR(1, 0.5f, 0.5f, 1));

	// ゲームパッドの場合はスティックの動きを反映
	uint8 type = ctrl_->GetCtrlNum();
	if (type == Controller::CtrlNum::PS4 ||
		type == Controller::CtrlNum::X)
	{
		short x = ctrl_->GetAxis().stickLX;
		short y = ctrl_->GetAxis().stickLY;
		if (Abs(x) > THRESHOLD || Abs(y) > THRESHOLD)
		{
			VECTOR2 pos = defaultPosition;
			pos.x += x * 0.1f;
			pos.y += y * 0.1f;
			stick_->SetPosition(pos);
		}
	}

	// チュートリアル判定
	if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) != 0 || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W) != 0)
	{
		stick_->SetColor(COLOR(1, 0, 0, 1));
		if (cnt_ >= 0) { cnt_++; }
	}

	UpdateTimer();
	if (Finish()) { return new Tutorial02Camera; }

	return nullptr;
}
