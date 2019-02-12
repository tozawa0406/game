#include "Tutorial05Item.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Systems/Input/KeyInput.h>

#include "Tutorial06Attack.h"

Tutorial05Item::Tutorial05Item(void) : 
	effectCnt_(0)
{
}

Tutorial05Item::~Tutorial05Item(void)
{
}

void Tutorial05Item::Init(TutorialManager* manager, Controller* ctrl)
{
	if (!manager) { return; }

	maxCnt_ = 3;
	TutorialBase::Init(manager, ctrl);	

	Resources::Texture::Camp texNum[3] = { Resources::Texture::Camp::UI_KEY_R, Resources::Texture::Camp::UI_KEY_T, Resources::Texture::Camp::UI_KEY_Y };
	for (int i = 0; i < 3; ++i)
	{
		key_[i].Init(211, static_cast<int>(texNum[i]));
		key_[i].SetPosition(TutorialManager::POSITION_KEYBOARD);
		key_[i].SetSize(TutorialManager::SIZE_KEY);
		key_[i].SetEnable(false);
	}

	text_.Init(211, "どのアイテムを使おうかにゃ……", FONT_SIZE);
	text_.SetPosition(TEXT_POSITION);
	text_.SetDrawSize(TEXT_DRAW_SIZE);
	text_.SetColor(COLOR(0, 0, 0));
	text_.SetFaldBack(true);
}

void Tutorial05Item::Uninit(void)
{
	for (auto& key : key_) { key.Uninit(); }

	TutorialBase::Uninit();
}

TutorialBase* Tutorial05Item::Update(void)
{
	if (!ctrl_) { return nullptr; }

	JedgeCtrlType();

	// デフォルトの設定
	key_[0].SetColor(COLOR_WAIT);
	for (int i = 1; i < 3; ++i) { key_[i].SetColor(COLOR(1)); }

	// チュートリアル判定
	if (ctrl_->Press(Input::GAMEPAD_L1, DIK_R))
	{
		key_[0].SetColor(COLOR_PUSH);
		for (int i = 1; i < 3; ++i) { key_[i].SetColor(COLOR_WAIT); }

		if (ctrl_->Trigger(Input::GAMEPAD_SQUARE, DIK_T))
		{
			if (cnt_ >= 0) { cnt_++; }
			effectCnt_++;		
		}
		else if (ctrl_->Trigger(Input::GAMEPAD_CIRCLE, DIK_Y))
		{
			if (cnt_ >= 0) { cnt_++; }
			effectCnt_ = 5;
		}
	}

	if (effectCnt_ > 0)
	{
		key_[(effectCnt_ / 5) + 1].SetColor(COLOR_PUSH);
		effectCnt_++;
		if (effectCnt_ % 5 == 0) { effectCnt_ = 0; }
	}

	UpdateTimer();
	if (Finish()) { return new Tutorial06Attack; }

	return nullptr;
}

void Tutorial05Item::JedgeCtrlType(void)
{
	if (!ctrl_) { return; }

	uint8 type = ctrl_->GetCtrlNum();

	Resources::Texture::Camp texNum[3] = { Resources::Texture::Camp::UI_KEY_R, Resources::Texture::Camp::UI_KEY_T, Resources::Texture::Camp::UI_KEY_Y };
	VECTOR2 pos		= TutorialManager::POSITION_KEYBOARD;
	VECTOR2 size	= TutorialManager::SIZE_KEY;
	switch (type)
	{
	case Controller::CtrlNum::PS4:
		pos			= TutorialManager::POSITION;
		size		= TutorialManager::SIZE_PS4;
		texNum[0]	= Resources::Texture::Camp::UI_PS4CTRL_L1;
		texNum[1]	= Resources::Texture::Camp::UI_PS4CTRL_SQUARE;
		texNum[2]	= Resources::Texture::Camp::UI_PS4CTRL_CIRCLE;
		break;
	case Controller::CtrlNum::X:
		pos			= TutorialManager::POSITION;
		size		= TutorialManager::SIZE_X;
		texNum[0]	= Resources::Texture::Camp::UI_XCTRL_LB;
		texNum[1]	= Resources::Texture::Camp::UI_XCTRL_X;
		texNum[2]	= Resources::Texture::Camp::UI_XCTRL_B;
		break;
	}

	for (int i = 0; i < 3; ++i)
	{
		key_[i].SetTexNum(static_cast<int>(texNum[i]));
		key_[i].SetPosition(pos);
		key_[i].SetSize(size);
		key_[i].SetEnable(true);
	}
}
