#include "Timer.h"

// �f�o�b�O�p
#ifdef _DEBUG
// ���Ԃ̌v����b�P�ʂɕύX����
//#define TIME_TO_SECOND
#endif

//! @def	�����ʒu
static const	 VECTOR2 POSITION	= VECTOR2(Windows::WIDTH * 0.1f, Windows::HEIGHT * 0.1f);
//! @def	�`�揇
static constexpr uint8 LAYER		= 100;
//! @def	�S�̂̃X�P�[��
static constexpr float SCALE		= 0.8f;
//! @def	�S�̂̓����x
static constexpr uint8 ALPHA		= 1;
//! @def	���v�̍ő��60
static constexpr uint8 TIME			= 60;
//! @def	�ŏ��̎���
static constexpr uint8 START_TIME	= 50;
//! @def	�ő劄��
static constexpr int   MAX_PERCENT	= 100;
//! @def	������
static constexpr float MOVE			= 0.0628f;
//! @def	�Ԃ��Ȃ�^�C�~���O
static constexpr int   RED_TIME		= 10;

//! @def	�w�i�̈ʒu����
static constexpr uint8 ADJUST_BACK_POSITION = 10;
//! @def	�w�i�̑傫��
static constexpr float BACK_SIZE	= 150;
//! @def	�w�i�̕�����
static const	 VECTOR2 BACK_SPLIT = VECTOR2(2, 1);

//! @def	�Q�[�W�̑傫��
static constexpr float   GAUGE_SIZE	= 100;

//! @def	�j�摜�̕�����
static const	 VECTOR2 HAND_SPLIT = VECTOR2(6, 1);
//! @def	�j�̑傫��
static const	 VECTOR2 HAND_SIZE	= VECTOR2(40, 65);

//! @def	�I���j�̈ʒu����
static constexpr float ADJUST_HANDEND_POSITION = -1;
//! @def	�I���j�̃e�N�X�`���ʒu
static constexpr uint8	 HANDEND_PATTERN	= 3;

//! @def	���ݐj�̈ʒu����
static constexpr float	 ADJUST_HANDNOW_POSITION = -0.85f;
//! @def	�I���j�̃e�N�X�`���ʒu
static constexpr uint8	 HANDNOW_PATTERN = 4;

//! @def	���v�������ߋ�ʒu����
static constexpr float	 ADJUST_CLIP_POSITION = -11.5f;
//! @def	���v�������ߋ�̑傫��
static const	 VECTOR2 CLIP_SIZE = VECTOR2(50, 150);
//! @def	���v�������ߋ�̃e�N�X�`���ʒu
static constexpr uint8	 CLIP_PATTERN = 5;

Timer::Timer(void) : Object(ObjectTag::UI), GUI(Systems::Instance(), this, "Timer")
	, frame_(0)
	, time_(START_TIME)
	, second_(0)
{
}

Timer::~Timer(void)
{
}

void Timer::Init(void)
{
	// �ʒu���w��
	transform_.position = POSITION;
	VECTOR2 pos = VECTOR2(POSITION.x, POSITION.y);

	// �w�i
	back_.Init(LAYER - 1, static_cast<int>(Texture::Game::TIMER_UI));
	back_.SetPosition(VECTOR2(pos.x - ADJUST_BACK_POSITION, pos.y));
	back_.SetSize(VECTOR2(BACK_SIZE));
	back_.SetColor(COLOR(1, 1, 1, 1 * ALPHA));
	back_.SetScale(VECTOR2(SCALE));
	back_.SetScaleOffset(VECTOR2(Half(BACK_SIZE)));
	back_.SetSplit(BACK_SPLIT);
	back_.SetPattern(0);

	// �Q�[�W�̏�����
	circleGauge_.Init(LAYER, static_cast<int>(Texture::Base::WHITE));
	circleGauge_.SetSize(VECTOR2(GAUGE_SIZE));
	circleGauge_.SetPosition(pos);
	circleGauge_.SetColor(COLOR(1, 1, 1, 0.5f * ALPHA));
	circleGauge_.SetScale(VECTOR2(SCALE));
	circleGauge_.SetScaleOffset(VECTOR2(Half(circleGauge_.GetSize())));
	circleGauge_.SetShader(Shader::ENUM::CIRCLE_GAUGE);
	circleGauge_.SetAngle((float)time_ / TIME * MAX_PERCENT);

	// �I���ʒu�j������
	handEnd_.Init(LAYER + 2, static_cast<int>(Texture::Game::TIMER_UI));
	handEnd_.SetSize(HAND_SIZE);
	handEnd_.SetPosition(VECTOR2(pos.x + ADJUST_HANDEND_POSITION, pos.y - Half(handEnd_.GetSize().y * SCALE)));
	handEnd_.SetColor(COLOR(1, 1, 1, ALPHA));
	handEnd_.SetScale(VECTOR2(SCALE));
	handEnd_.SetScaleOffset(VECTOR2(Half(handEnd_.GetSize())));
	handEnd_.SetRotationOffset(VECTOR2(Half(handEnd_.GetSize().x), handEnd_.GetSize().y + (Half(handEnd_.GetSize().y * SCALE - handEnd_.GetSize().y))));
	handEnd_.SetSplit(HAND_SPLIT);
	handEnd_.SetPattern(HANDEND_PATTERN);

	// ���݈ʒu�j������
	handNow_.Init(LAYER + 1, (int)Texture::Game::TIMER_UI);
	handNow_.SetSize(HAND_SIZE);
	handNow_.SetPosition(VECTOR2(pos.x + ADJUST_HANDNOW_POSITION, pos.y - Half(handNow_.GetSize().y * SCALE)));
	handNow_.SetColor(COLOR(1, 0, 0, ALPHA));
	handNow_.SetScale(VECTOR2(SCALE));
	handNow_.SetAngle(circleGauge_.GetAngle() * MOVE);
	handNow_.SetScaleOffset(VECTOR2(Half(handNow_.GetSize())));
	handNow_.SetRotationOffset(VECTOR2(Half(handNow_.GetSize().x), handNow_.GetSize().y + (Half(handNow_.GetSize().y * SCALE - handNow_.GetSize().y))));
	handNow_.SetSplit(HAND_SPLIT);
	handNow_.SetPattern(HANDNOW_PATTERN);

	// ���v����
	clip_.Init(LAYER + 4, (int)Texture::Game::TIMER_UI);
	clip_.SetSize(CLIP_SIZE);
	clip_.SetPosition(VECTOR2(pos.x + ADJUST_CLIP_POSITION, pos.y));
	clip_.SetColor(COLOR(1, 1, 1, 1 * ALPHA));
	clip_.SetScale(VECTOR2(SCALE));
	clip_.SetScaleOffset(VECTOR2(Half(back_.GetSize())));
	clip_.SetSplit(HAND_SPLIT);
	clip_.SetPattern(CLIP_PATTERN);
}

void Timer::Uninit(void)
{
	clip_.Uninit();
	handNow_.Uninit();
	handEnd_.Uninit();
	circleGauge_.Uninit();
	back_.Uninit();
}

void Timer::Update(void)
{
#ifdef TIME_TO_SECOND
#else
	frame_++;
	// ��b�o��
	if (frame_ > TIME)
#endif
	{
		frame_ = 0;
		second_++;
		if (second_ > TIME)
		{
			second_ = 0;
			time_--;
		}
	}

	// �p�x��ύX
	float angle = (float)time_ / TIME * MAX_PERCENT;
	circleGauge_.SetAngle(angle);

	// �j����
	handNow_.SetAngle(max(0, (angle * MOVE)));
	// ���𒴂������ԏ�ɕ`��
	if (handNow_.GetAngle() < PI) { handNow_.SetPriority(LAYER + 3); }

	// �c�菭���ŐԂ�����
	if (angle < RED_TIME)
	{
		circleGauge_.SetColor(COLOR(1, 0, 0, 0.5f * ALPHA));
	}

}

void Timer::GuiUpdate(void)
{
	ImGui::Text("remaining time : %d", time_);
	ImGui::Text("second         : %d", second_);
	ImGui::Text("frame          : %d", frame_);

	if (ImGui::Button("1 min pas"))
	{
		time_ -= 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("5 min pas"))
	{
		time_ -= 5;
	}
	ImGui::SameLine();
	if (ImGui::Button("10 min pas"))
	{
		time_ -= 10;
	}
}
