#include "TitleScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Windows/Windows.h>

//! @def	UI�`�揀
static constexpr int UI_PRIORITY = 200;
//! @def	press�`��̈ʒu
static const VECTOR2 PRESS_POSITION = VECTOR2((float)Half(Windows::WIDTH) - 50, Windows::HEIGHT * 0.9f);
//! @def	pree�`��̃T�C�Y
static const VECTOR2 PRESS_SIZE = VECTOR2((float)Quarter(Windows::WIDTH), Half(Windows::HEIGHT * 0.1f));
//! @def	UI�{�^���̕`��ʒu����
static constexpr int ADJUST_POSITION_X = 190;
//! @def	UI�{�^���L�[�{�[�h�̕`��ʒu����
static constexpr int ADJUST_KEY_POSITION_X = 50;
//! @def	UI�{�^���L�[�{�[�h�̕`��T�C�Y����
static constexpr int ADJUST_KEY_SIZE_X = 4;

//! @def	�_�ŊԊu
static constexpr float FLASHING_RANGE = 60;

/* @brief	�R���X�g���N�^			*/
TitleScene::TitleScene(SceneManager* manager) : BaseScene(manager), GUI(manager->GetSystems(), nullptr, "SceneTitle")
	, frameCnt_(0)
{
}

/* @brief	�f�X�g���N�^			*/
TitleScene::~TitleScene(void)
{
}

/* @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void TitleScene::Init(void)
{
	// �uplease press�v
	press_.Init(UI_PRIORITY, static_cast<int>(Texture::Title::PLEASE_PRESS));
	press_.SetPosition(PRESS_POSITION);
	press_.SetSize(PRESS_SIZE);

	// �e��{�^��
	for (auto& b : button_)
	{
		b.Init(UI_PRIORITY, static_cast<int>(Texture::Title::ENTER));
		b.SetPosition(VECTOR2(PRESS_POSITION.x + ADJUST_POSITION_X, PRESS_POSITION.y));
		b.SetSize(VECTOR2(PRESS_SIZE.y, PRESS_SIZE.y));
		b.SetEnable(false);
	}
	int num = static_cast<int>(InputType::Keyboard);
	auto pos = button_[num].GetPosition();
	pos.x += ADJUST_KEY_POSITION_X;
	button_[num].SetPosition(pos);
	auto size = button_[num].GetSize();
	size.x *= ADJUST_KEY_SIZE_X;
	button_[num].SetSize(size);

	num = static_cast<int>(InputType::PS4);
	button_[num].SetTexNum(static_cast<int>(Texture::Title::MARU));

	num = static_cast<int>(InputType::X);
	button_[num].SetTexNum(static_cast<int>(Texture::Title::B));
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void TitleScene::Uninit(void)
{
	for (auto& b : button_)
	{
		b.Uninit();
	}
	press_.Uninit();
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	�V�[���ԍ�			*/
SceneList TitleScene::Update(void)
{
	// �R���g���[���̎擾
	const auto& ctrl = GetCtrl(0);
	if (!ctrl) { return SceneList::NOTCHANGE; }

	// �_��
	Flashing(*ctrl);

	// �J�ڏ���
	return EndScene(*ctrl);
}

/* @brief	�_�ŏ���
 * @sa		Update()
 * @param	(ctrl)	�R���g���[��
 * @return	�Ȃ�				*/
void TitleScene::Flashing(Controller& ctrl)
{
	// �t���[���J�E���^
	frameCnt_++;
	if (frameCnt_ > FLASHING_RANGE * 2) { frameCnt_ = 0; }

	// ���̐ݒ�
	float a = (frameCnt_ < FLASHING_RANGE) ? frameCnt_ / FLASHING_RANGE : 2 - frameCnt_ / FLASHING_RANGE;
	auto c = press_.GetColor();
	c.a = a;
	press_.SetColor(c);
	c = button_[JudgeCtrlType(ctrl)].GetColor();
	c.a = a;
	button_[JudgeCtrlType(ctrl)].SetColor(c);
}

/* @brief	�R���g���[���̃^�C�v����
 * @sa		Flashing()
 * @param	(ctrl)	�R���g���[��
 * @return	�R���g���[���̃^�C�v�ɂ��z��ԍ�	*/
int TitleScene::JudgeCtrlType(Controller& ctrl)
{
	int num = 0;
	switch (static_cast<Controller::CtrlNum>(ctrl.GetCtrlNum()))
	{
	case Controller::CtrlNum::Key:
		num = static_cast<int>(InputType::Keyboard);
		break;
	case Controller::CtrlNum::PS4:
		num = static_cast<int>(InputType::PS4);
		break;
	case Controller::CtrlNum::X:
		num = static_cast<int>(InputType::X);
		break;
	}
	for (auto& b : button_) { b.SetEnable(false); }
	button_[num].SetEnable(true);

	return num;
}

/* @brief	�J�ڏ���
 * @sa		Update()
 * @param	(ctrl)	�R���g���[��
 * @return	�V�[���ԍ�			*/
SceneList TitleScene::EndScene(Controller& ctrl)
{
	// ����
	if (ctrl.Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		return SceneList::NEXT;
	}

	return SceneList::NOTCHANGE;
}

/* @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void TitleScene::GuiUpdate(void)
{
	ImGui::Text("a : %.2f", (frameCnt_ < FLASHING_RANGE) ? frameCnt_ / FLASHING_RANGE : 2 - frameCnt_ / FLASHING_RANGE);
}
