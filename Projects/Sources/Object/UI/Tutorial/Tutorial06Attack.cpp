#include "Tutorial06Attack.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Systems/Input/KeyInput.h>
#include "../../GameObject/Player/Player.h"

#include "TutorialFormal.h"

static constexpr int CHANGE_UI = 4;

Tutorial06Attack::Tutorial06Attack(void) : 
	effectCnt_(0)
	, player_(nullptr)
{
}

Tutorial06Attack::~Tutorial06Attack(void)
{
}

void Tutorial06Attack::Init(TutorialManager* manager, Controller* ctrl)
{
	if (!manager) { return; }

	maxCnt_ = CHANGE_UI + 1;
	TutorialBase::Init(manager, ctrl);

	player_ = manager->GetPlayer();

	key_.Init(211, static_cast<int>(Resources::Texture::Camp::UI_KEY_U));
	key_.SetPosition(TutorialManager::POSITION_KEYBOARD);
	key_.SetSize(TutorialManager::SIZE_KEY);
	key_.SetEnable(false);

	text_.Init(211, "これで攻撃するにゃ", FONT_SIZE);
	text_.SetPosition(TEXT_POSITION);
	text_.SetDrawSize(TEXT_DRAW_SIZE);
	text_.SetColor(COLOR(0, 0, 0));
	text_.SetFaldBack(true);

	manager->SetCharactorPattern(2);
}

void Tutorial06Attack::Uninit(void)
{
	key_.Uninit();

	TutorialBase::Uninit();
}

TutorialBase* Tutorial06Attack::Update(void)
{
	if (!ctrl_ || !player_) { player_ = manager_->GetPlayer(); return nullptr; }

	JedgeCtrlType();

	// デフォルトの設定
	key_.SetColor(COLOR_WAIT);

	// チュートリアル判定
	WORD	gamepad = (cnt_ < CHANGE_UI) ? Input::GAMEPAD_TRIANGLE: Input::GAMEPAD_SQUARE;
	int		key		= (cnt_ < CHANGE_UI) ? DIK_U : DIK_H;
	if (ctrl_->Trigger(gamepad, key))
	{
		if (AddCount(player_->GetAnimation()))
		{
			effectCnt_++;
			cnt_++;
		}
	}

	if (effectCnt_ > 0)
	{
		key_.SetColor(COLOR_PUSH);
		effectCnt_++;
		if (effectCnt_ > 5) { effectCnt_ = 0; }
	}

	UpdateTimer();
	if (Finish()) { return new TutorialFormal; }

	return nullptr;
}

void Tutorial06Attack::JedgeCtrlType(void)
{
	if (!ctrl_) { return; }

	uint8 type = ctrl_->GetCtrlNum();

	Resources::Texture::Camp texNum = (cnt_ < CHANGE_UI) ? Resources::Texture::Camp::UI_KEY_U : Resources::Texture::Camp::UI_KEY_H;
	VECTOR2 pos		= TutorialManager::POSITION_KEYBOARD;
	VECTOR2 size	= TutorialManager::SIZE_KEY;
	switch (type)
	{
	case Controller::CtrlNum::PS4:
		pos		= TutorialManager::POSITION;
		size	= TutorialManager::SIZE_PS4;
		texNum	= (cnt_ < CHANGE_UI) ? Resources::Texture::Camp::UI_PS4CTRL_TRIANGLE : Resources::Texture::Camp::UI_PS4CTRL_SQUARE;
		break;
	case Controller::CtrlNum::X:
		pos		= TutorialManager::POSITION;
		size	= TutorialManager::SIZE_X;
		texNum	= (cnt_ < CHANGE_UI) ? Resources::Texture::Camp::UI_XCTRL_Y : Resources::Texture::Camp::UI_XCTRL_X;
		break;
	}

	key_.SetTexNum(static_cast<int>(texNum));
	key_.SetPosition(pos);
	key_.SetSize(size);
	key_.SetEnable(true);
}

bool Tutorial06Attack::AddCount(int playerAnim)
{
	Player::Animation anim = static_cast<Player::Animation>(playerAnim);
	// 抜刀
	if (cnt_ == 0)
	{
		if ((anim == Player::Animation::Wait ||
		   (anim == Player::Animation::WaitTime1 || anim == Player::Animation::WaitTime2)) ||
		   (anim == Player::Animation::Run  || anim == Player::Animation::Walk))
		{
			return true;
		}
	}
	// 最初の攻撃
	else if (cnt_ == 1)
	{
		if (anim == Player::Animation::SetupDrawn)
		{
			// 抜刀斬りの場合は1ステップ進める
			cnt_++;
			return true;
		}
		else if (anim == Player::Animation::SetupWait || anim == Player::Animation::SetupWalk)
		{
			return true;
		}
	}
	// 攻撃
	else if (cnt_ < CHANGE_UI)
	{
		int judge = static_cast<int>(Player::Animation::Slash_1) + (cnt_ - 2);
		if (anim == static_cast<Player::Animation>(judge))
		{
			return true;
		}
	}
	// 最後は納刀
	else if (cnt_ == CHANGE_UI)
	{
		if (anim == Player::Animation::SetupWait || anim == Player::Animation::SetupWalk)
		{
			return true;
		}
	}

	return false;
}
