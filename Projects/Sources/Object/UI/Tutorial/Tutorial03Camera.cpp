#include "Tutorial03Camera.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Systems/Input/KeyInput.h>

static constexpr short THRESHOLD = 10;

Tutorial03Camera::Tutorial03Camera(void) :
	stick_(nullptr)
{
}

Tutorial03Camera::~Tutorial03Camera(void)
{
}

void Tutorial03Camera::Init(TutorialManager* manager, Controller* ctrl)
{
	if (!manager) { return; }

	maxCnt_ = 3 * 60;
	TutorialBase::Init(manager, ctrl);	

	stick_ = manager_->GetStickUIPtr(true);
	if (stick_)
	{
		stick_->SetColor(COLOR(1, 0.5f, 0.5f, 1));
	}

	Resources::Texture::Camp texNum[4] = { Resources::Texture::Camp::UI_KEY_I, Resources::Texture::Camp::UI_KEY_J, Resources::Texture::Camp::UI_KEY_K, Resources::Texture::Camp::UI_KEY_L };
	for (int i = 0; i < 4; ++i)
	{
		key_[i].Init(211, static_cast<int>(texNum[i]));
		key_[i].SetPosition(TutorialManager::POSITION_KEYBOARD);
		key_[i].SetSize(TutorialManager::SIZE_KEY);
		key_[i].SetEnable(false);
	}
}

void Tutorial03Camera::Uninit(void)
{
	for (auto& key : key_) { key.Uninit(); }
	stick_->SetPosition(TutorialManager::POSITION);
	stick_->SetColor(COLOR(1, 1, 1, 0.75f));

	TutorialBase::Uninit();
}

TutorialBase* Tutorial03Camera::Update(void)
{
	if (!ctrl_ || !stick_) { return nullptr; }

	// デフォルトの設定
	stick_->SetColor(COLOR(1, 0.5f, 0.5f, 1));
	for (auto& key : key_) { key.SetColor(COLOR(1, 0.5f, 0.5f, 1)); }

	// ゲームパッドの場合はスティックの動きを反映
	uint8 type = ctrl_->GetCtrlNum();
	if (type == Controller::CtrlNum::PS4 ||
		type == Controller::CtrlNum::X)
	{
		short x = ctrl_->GetAxis().stickRX;
		short y = ctrl_->GetAxis().stickRY;
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
	if (ctrl_->PressRange(Input::AXIS_RX, DIK_J, DIK_L) != 0 || ctrl_->PressRange(Input::AXIS_RY, DIK_K, DIK_I) != 0)
	{
		stick_->SetColor(COLOR(1, 0, 0, 1));
		if (cnt_ >= 0) { cnt_++; }
		if (const auto& input = ctrl_->GetInput())
		{
			if (const auto& key = input->GetKeyboard())
			{
				if		(key->Press(DIK_I)) { key_[0].SetColor(COLOR(1, 0, 0, 1)); }
				else if (key->Press(DIK_J)) { key_[1].SetColor(COLOR(1, 0, 0, 1)); }
				else if (key->Press(DIK_K)) { key_[2].SetColor(COLOR(1, 0, 0, 1)); }
				else if (key->Press(DIK_L)) { key_[3].SetColor(COLOR(1, 0, 0, 1)); }
			}
		}

	}

	UpdateTimer();
	if (Finish()) { return nullptr; }

	return nullptr;
}
