#include "Tutorial02Dash.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Systems/Input/KeyInput.h>

#include "Tutorial03Camera.h"

static constexpr short THRESHOLD = 10;

Tutorial02Dash::Tutorial02Dash(void) :
	stick_(nullptr)
{
}

Tutorial02Dash::~Tutorial02Dash(void)
{
}

void Tutorial02Dash::Init(TutorialManager* manager, Controller* ctrl)
{
	if (!manager) { return; }

	maxCnt_ = 2 * 60;
	TutorialBase::Init(manager, ctrl);	

	stick_ = manager_->GetStickUIPtr(false);
	if (stick_)
	{
		stick_->SetColor(COLOR(1, 0.5f, 0.5f, 1));
	}

	Resources::Texture::Camp texNum[4] = { Resources::Texture::Camp::UI_KEY_A, Resources::Texture::Camp::UI_KEY_W, Resources::Texture::Camp::UI_KEY_S, Resources::Texture::Camp::UI_KEY_D };
	for (int i = 0; i < 4; ++i)
	{
		key_[i].Init(211, static_cast<int>(texNum[i]));
		key_[i].SetPosition(TutorialManager::POSITION_KEYBOARD);
		key_[i].SetSize(TutorialManager::SIZE_KEY);
		key_[i].SetEnable(false);
	}
	dash_.Init(211, static_cast<int>(Resources::Texture::Camp::UI_KEY_LSHIFT));
	dash_.SetPosition(TutorialManager::POSITION_KEYBOARD);	
	dash_.SetSize(TutorialManager::SIZE_KEY);
	dash_.SetColor(COLOR(1, 0.5f, 0.5f, 1));
}

void Tutorial02Dash::Uninit(void)
{
	dash_.Uninit();
	for (auto& key : key_) { key.Uninit(); }
	stick_->SetPosition(TutorialManager::POSITION);
	stick_->SetColor(COLOR(1, 1, 1, 0.75f));

	TutorialBase::Uninit();
}

TutorialBase* Tutorial02Dash::Update(void)
{
	if (!ctrl_ || !stick_) { return nullptr; }

	uint8 type = ctrl_->GetCtrlNum();
	InputDifference(type);

	// �f�t�H���g�̐ݒ�
	dash_.SetColor(COLOR(1, 0.5f, 0.5f, 1));
	stick_->SetColor(COLOR(1));
	for (auto& key : key_) { key.SetColor(COLOR(1)); }

	// �`���[�g���A������
	if (ctrl_->Press(Input::GAMEPAD_R1, DIK_LSHIFT))
	{
		dash_.SetColor(COLOR(1, 0, 0, 1));
		stick_->SetColor(COLOR(1, 0.5f, 0.5f, 1));
		for (auto& key : key_) { key.SetColor(COLOR(1, 0.5f, 0.5f, 1)); }

		// �Q�[���p�b�h�̏ꍇ�̓X�e�B�b�N�̓����𔽉f
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
		}

		if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) != 0 || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W) != 0)
		{
			stick_->SetColor(COLOR(1, 0, 0, 1));
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
	}

	UpdateTimer();
	if (Finish()) { return new Tutorial03Camera; }

	return nullptr;
}

void Tutorial02Dash::InputDifference(uint8 type)
{
	for (auto& key : key_) { key.SetEnable((type == Controller::CtrlNum::Key) ? true : false); }

	switch (type)
	{
	case Controller::CtrlNum::Key:
		dash_.SetTexNum(static_cast<int>(Resources::Texture::Camp::UI_KEY_LSHIFT));
		dash_.SetPosition(TutorialManager::POSITION_KEYBOARD);
		dash_.SetSize(TutorialManager::SIZE_KEY);
		break;
	case Controller::CtrlNum::PS4:
		dash_.SetPosition(TutorialManager::POSITION);
		dash_.SetTexNum(static_cast<int>(Resources::Texture::Camp::UI_PS4CTRL_R1));
		dash_.SetSize(TutorialManager::SIZE_PS4);
		break;
	case Controller::CtrlNum::X:
		dash_.SetPosition(TutorialManager::POSITION);
		dash_.SetTexNum(static_cast<int>(Resources::Texture::Camp::UI_XCTRL_RB));
		dash_.SetSize(TutorialManager::SIZE_X);
		break;
	}
}
