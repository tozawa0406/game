#include "Tutorial04Avoidance.h"
#include <FrameWork/Systems/Input/Controller.h>
#include <FrameWork/Systems/Input/KeyInput.h>
#include "../../GameObject/Player/Player.h"

#include "Tutorial05Item.h"

Tutorial04Avoidance::Tutorial04Avoidance(void) : 
	effectCnt_(0)
	, player_(nullptr)
{
}

Tutorial04Avoidance::~Tutorial04Avoidance(void)
{
}

void Tutorial04Avoidance::Init(TutorialManager* manager, Controller* ctrl)
{
	if (!manager) { return; }

	maxCnt_ = 3;
	TutorialBase::Init(manager, ctrl);	

	key_.Init(211, static_cast<int>(Resources::Texture::Camp::UI_KEY_M));
	key_.SetPosition(TutorialManager::POSITION_KEYBOARD);
	key_.SetSize(TutorialManager::SIZE_KEY);
	key_.SetEnable(false);

	player_ = manager->GetPlayer();
}

void Tutorial04Avoidance::Uninit(void)
{
	key_.Uninit();

	TutorialBase::Uninit();
}

TutorialBase* Tutorial04Avoidance::Update(void)
{
	if (!ctrl_ || !player_) { return nullptr; }

	JudgeCtrlType();

	// デフォルトの設定
	key_.SetColor(COLOR(1, 0.5f, 0.5f, 1));

	// チュートリアル判定
	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		Player::Animation anim = static_cast<Player::Animation>(player_->GetAnimation());
		if (anim != Player::Animation::Roll && anim != Player::Animation::Dive)
		{
			if (cnt_ >= 0) { cnt_++; }
			effectCnt_++;
		}
	}

	if (effectCnt_ > 0)
	{
		effectCnt_++;
		key_.SetColor(COLOR(1, 0, 0, 1));
		if (effectCnt_ > 5) { effectCnt_ = 0; }
	}

	UpdateTimer();
	if (Finish()) { return new Tutorial05Item; }

	return nullptr;
}

void Tutorial04Avoidance::JudgeCtrlType(void)
{
	if (!ctrl_) { return; }

	Resources::Texture::Camp texNum = Resources::Texture::Camp::UI_KEY_M;
	VECTOR2 pos		= TutorialManager::POSITION_KEYBOARD;
	VECTOR2 size	= TutorialManager::SIZE_KEY;

	switch (ctrl_->GetCtrlNum())
	{
	case Controller::CtrlNum::PS4:
		pos		= TutorialManager::POSITION;
		texNum	= Resources::Texture::Camp::UI_PS4CTRL_CROSS;
		size	= TutorialManager::SIZE_PS4;
		break;
	case Controller::CtrlNum::X:
		pos		= TutorialManager::POSITION;
		texNum	= Resources::Texture::Camp::UI_XCTRL_A;
		size	= TutorialManager::SIZE_X;
		break;
	}

	key_.SetPosition(pos);
	key_.SetTexNum(static_cast<int>(texNum));
	key_.SetSize(size);
	key_.SetEnable(true);
}
