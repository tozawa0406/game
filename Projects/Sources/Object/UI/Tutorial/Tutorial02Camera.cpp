#include "Tutorial02Camera.h"
#include <FrameWork/Systems/Input/Controller.h>

static constexpr short THRESHOLD = 10;

Tutorial02Camera::Tutorial02Camera(void) :
	stick_(nullptr)
	, defaultPosition(VECTOR2(0))
{
}

Tutorial02Camera::~Tutorial02Camera(void)
{
}

void Tutorial02Camera::Init(TutorialManager* manager, Controller* ctrl)
{
	if (!manager) { return; }

	maxCnt_ = 3 * 60;
	TutorialBase::Init(manager, ctrl);	

	stick_ = manager_->GetStickUIPtr(true);
	if (stick_)
	{
		defaultPosition = stick_->GetPosition();
		stick_->SetColor(COLOR(1, 0.5f, 0.5f, 1));
	}
}

void Tutorial02Camera::Uninit(void)
{
	stick_->SetPosition(defaultPosition);
	stick_->SetColor(COLOR(1, 1, 1, 0.75f));

	TutorialBase::Uninit();
}

TutorialBase* Tutorial02Camera::Update(void)
{
	if (!ctrl_ || !stick_) { return nullptr; }

	stick_->SetPosition(defaultPosition);
	stick_->SetColor(COLOR(1, 0.5f, 0.5f, 1));

	// ゲームパッドの場合はスティックの動きを反映
	uint8 type = ctrl_->GetCtrlNum();
	if (type == Controller::CtrlNum::PS4 ||
		type == Controller::CtrlNum::X)
	{
		short x = ctrl_->GetAxis().stickRX;
		short y = ctrl_->GetAxis().stickRY;
		if (Abs(x) > THRESHOLD || Abs(y) > THRESHOLD)
		{
			VECTOR2 pos = defaultPosition;
			pos.x += x * 0.1f;
			pos.y += y * 0.1f;
			stick_->SetPosition(pos);
		}
	}

	// チュートリアル判定
	if (ctrl_->PressRange(Input::AXIS_RX, DIK_J, DIK_L) != 0 || ctrl_->PressRange(Input::AXIS_RY, DIK_K, DIK_I) != 0)
	{
		stick_->SetColor(COLOR(1, 0, 0, 1));
		if (cnt_ >= 0) { cnt_++; }
	}

	UpdateTimer();
	if (Finish()) { return nullptr; }

	return nullptr;
}
