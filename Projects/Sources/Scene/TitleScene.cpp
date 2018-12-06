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
	press_.Init(systems_, UI_PRIORITY, static_cast<int>(Texture::Title::PLEASE_PRESS));
	press_.position = PRESS_POSITION;
	press_.size		= PRESS_SIZE;

	// �e��{�^��
	for (auto& b : button_)
	{
		b.Init(systems_, UI_PRIORITY, static_cast<int>(Texture::Title::ENTER));
		b.position	  = press_.position;
		b.position.x += ADJUST_POSITION_X;
		b.size		  = VECTOR2(press_.size.y, press_.size.y);
		b.enable	  = false;
	}
	int num = static_cast<int>(InputType::Keyboard);
	button_[num].position.x += ADJUST_KEY_POSITION_X;
	button_[num].size.x		*= ADJUST_KEY_SIZE_X;

	num = static_cast<int>(InputType::PS4);
	button_[num].texNum = static_cast<int>(Texture::Title::MARU);

	num = static_cast<int>(InputType::X);
	button_[num].texNum = static_cast<int>(Texture::Title::B);
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void TitleScene::Uninit(void)
{
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
	press_.color.a = a;
	button_[JudgeCtrlType(ctrl)].color.a = a;
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
	for (auto& b : button_) { b.enable = false; }
	button_[num].enable = true;

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
