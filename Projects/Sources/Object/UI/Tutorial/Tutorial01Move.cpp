#include "Tutorial01Move.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Systems/Input/KeyInput.h>

#include "Tutorial02Dash.h"

static constexpr short THRESHOLD = 10;

Tutorial01Move::Tutorial01Move(void) :
	stick_(nullptr)
{
}

Tutorial01Move::~Tutorial01Move(void)
{
}

void Tutorial01Move::Init(TutorialManager* manager, Controller* ctrl)
{
	if (!manager) { return; }

	maxCnt_ = 2 * 60;
	TutorialBase::Init(manager, ctrl);	

	stick_ = manager_->GetStickUIPtr(false);
	if (stick_)
	{
		stick_->SetColor(COLOR_WAIT);
	}

	Resources::Texture::Camp texNum[4] = { Resources::Texture::Camp::UI_KEY_A, Resources::Texture::Camp::UI_KEY_W, Resources::Texture::Camp::UI_KEY_S, Resources::Texture::Camp::UI_KEY_D };
	for (int i = 0; i < 4; ++i)
	{
		key_[i].Init(211, static_cast<int>(texNum[i]));
		key_[i].SetPosition(TutorialManager::POSITION_KEYBOARD);
		key_[i].SetSize(TutorialManager::SIZE_KEY);
		key_[i].SetEnable(false);
	}

	text_.Init(211, "移動してみるにゃ", FONT_SIZE);
	text_.SetPosition(TEXT_POSITION);
	text_.SetColor(COLOR(0, 0, 0));
	text_.SetFaldBack(true);
	text_.SetDrawSize(TEXT_DRAW_SIZE);
}

void Tutorial01Move::Uninit(void)
{
	for (auto& key : key_) { key.Uninit(); }
	stick_->SetPosition(TutorialManager::POSITION);
	stick_->SetColor(COLOR(1, 1, 1, 0.75f));

	TutorialBase::Uninit();
}

TutorialBase* Tutorial01Move::Update(void)
{
	if (!ctrl_ || !stick_) { return nullptr; }

	// デフォルトの設定
	stick_->SetColor(COLOR_WAIT);
	for (auto& key : key_) { key.SetColor(COLOR_WAIT); }

	// ゲームパッドの場合はスティックの動きを反映
	uint8 type = ctrl_->GetCtrlNum();
	if (type == Controller::CtrlNum::PS4 ||
		type == Controller::CtrlNum::X)
	{
		short x = ctrl_->GetAxis().stickLX;
		short y = ctrl_->GetAxis().stickLY;
		if (Abs(x) > THRESHOLD || Abs(y) > THRESHOLD)
		{
			VECTOR2 pos = TutorialManager::POSITION;
			pos.x += x * 0.1f;
			pos.y += y * 0.1f;
			stick_->SetPosition(pos);
		}
		for (auto& key : key_) { key.SetEnable(false); }
	}
	else
	{
		for (auto& key : key_) { key.SetEnable(true); }
	}

	// チュートリアル判定
	if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) != 0 || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W) != 0)
	{
		stick_->SetColor(COLOR_PUSH);
		if (cnt_ >= 0) { cnt_++; }

		if (const auto& input = ctrl_->GetInput())
		{
			if (const auto& key = input->GetKeyboard())
			{
				if		(key->Press(DIK_A)) { key_[0].SetColor(COLOR(1, 0, 0, 1)); }
				else if (key->Press(DIK_W)) { key_[1].SetColor(COLOR(1, 0, 0, 1)); }
				else if (key->Press(DIK_S)) { key_[2].SetColor(COLOR(1, 0, 0, 1)); }
				else if (key->Press(DIK_D)) { key_[3].SetColor(COLOR(1, 0, 0, 1)); }
			}
		}
	}

	UpdateTimer();
	if (Finish()) { return new Tutorial02Dash; }

	return nullptr;
}
