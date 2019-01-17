#include "05_ResultScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Windows/Windows.h>

//! @def	UI�`�揀
static constexpr int UI_PRIORITY = 200;
//! @def	UI�����T�C�Y
static constexpr int UI_TEXT_SIZE = 30;
//! @def	press�`��̈ʒu
static const VECTOR2 PRESS_POSITION = VECTOR2(Half(Windows::WIDTH) + 200, Windows::HEIGHT * 0.9f - UI_TEXT_SIZE);
//! @def	pree�`��̃T�C�Y
static const VECTOR2 PRESS_SIZE = VECTOR2(Quarter(Windows::WIDTH), Half(Windows::HEIGHT * 0.1f));
//! @def	UI�{�^���̕`��ʒu����
static constexpr float ADJUST_POSITION_X = 6 * UI_TEXT_SIZE + UI_TEXT_SIZE * 0.5f;

//! @def	�_�ŊԊu
static constexpr float FLASHING_RANGE = 60;

ResultScene::ResultScene(void) : GUI(Systems::Instance(), nullptr, "SceneResult")
	, frameCnt_(0)
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
	thanks_.Init(UI_PRIORITY, "Thank you for Playing!", 70);
	thanks_.SetPosition(VECTOR2(250, 300));

	// �uplease press�v
	press_.Init(UI_PRIORITY, "Please Press", UI_TEXT_SIZE);
	press_.SetPosition(PRESS_POSITION);

	// �e��{�^��
	button_.Init(UI_PRIORITY, "Enter �Z B", UI_TEXT_SIZE);
	button_.SetPosition(VECTOR2(PRESS_POSITION.x + ADJUST_POSITION_X, PRESS_POSITION.y));

	// �w�i
	back_.Init(UI_PRIORITY - 1, static_cast<int>(Resources::Texture::Base::WHITE));
	back_.SetPosition(VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT)));
	back_.SetSize(VECTOR2(Windows::WIDTH, Windows::HEIGHT));
	back_.SetColor(COLOR(0, 0, 0, 1));

	if (const auto& sound = GetSound())
	{
		sound->Play((int)Resources::Sound::Result::BGM_RESULT);
	}
}

void ResultScene::Uninit(void)
{
	if (const auto& sound = GetSound())
	{
		sound->Stop((int)Resources::Sound::Result::BGM_RESULT);
	}
	back_.Uninit();
	thanks_.Uninit();
	button_.Uninit();
	press_.Uninit();
}

SceneList ResultScene::Update(void)
{
	// �R���g���[���̎擾
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	// ���͔���
	JudgeCtrlType(*ctrl);
	// �_��
	Flashing();

	// �J�ڏ���
	return EndScene(*ctrl);
}

void ResultScene::Flashing(void)
{
	// �t���[���J�E���^
	frameCnt_++;
	if (frameCnt_ > FLASHING_RANGE * 2) { frameCnt_ = 0; }
	
	// ���̐ݒ�
	float a = (frameCnt_ < FLASHING_RANGE) ? frameCnt_ / FLASHING_RANGE : 2 - frameCnt_ / FLASHING_RANGE;
	auto c = press_.GetColor();
	c.a = a;
	press_.SetColor(c);
	c = button_.GetColor();
	c.a = a;
	button_.SetColor(c);
}

void ResultScene::JudgeCtrlType(Controller& ctrl)
{
	switch (static_cast<Controller::CtrlNum>(ctrl.GetCtrlNum()))
	{
	case Controller::CtrlNum::Key:
		button_.SetString("Enter");
		break;
	case Controller::CtrlNum::PS4:
		button_.SetString("�Z");
		break;
	case Controller::CtrlNum::X:
		button_.SetString("B");
		break;
	}
}

SceneList ResultScene::EndScene(Controller& ctrl)
{
	// ����
	if (ctrl.Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		if (const auto& sound = GetSound())
		{
			sound->Play(static_cast<int>(Resources::Sound::Base::SE_ENTER));
		}
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}

