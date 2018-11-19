#include "Timer.h"

// �f�o�b�O�p
#ifdef _DEBUG
// ���Ԃ̌v����b�P�ʂɕύX����
#define TIME_TO_SECOND
#endif

//! @def	�����ʒu
static const	 VECTOR2 POSITION	= VECTOR2(Graphics::WIDTH * 0.1f, Graphics::HEIGHT * 0.1f);
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



/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����		*/
Timer::Timer(void) : Object(Object::Tag::UI), frame_(0), time_(START_TIME), second_(0)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...					*/
Timer::~Timer(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void Timer::Init(void)
{
	// �ʒu���w��
	transform_.position = POSITION;

	const auto& systems = Systems::Instance();

	// �w�i
	back_.Init(systems, LAYER - 1, static_cast<int>(Texture::Game::TIMER_UI));
	back_.position				= transform_.position;
	back_.position.x		   -= ADJUST_BACK_POSITION;
	back_.size					= VECTOR2(BACK_SIZE);
	back_.color					= COLOR(1, 1, 1, 1 * ALPHA);
	back_.scale					= VECTOR2(SCALE);
	back_.scaleOffset			= VECTOR2(Half(back_.size));
	back_.split					= BACK_SPLIT;
	back_.pattern				= 0;

	// �Q�[�W�̏�����
	circleGauge_.Init(systems, LAYER, static_cast<int>(Texture::Base::WHITE));
	circleGauge_.position		= transform_.position;
	circleGauge_.size			= VECTOR2(GAUGE_SIZE);
	circleGauge_.color			= COLOR(1, 1, 1, 0.5f * ALPHA);
	circleGauge_.scale			= VECTOR2(SCALE);
	circleGauge_.scaleOffset	= VECTOR2(Half(circleGauge_.size));
	circleGauge_.shader			= Shader::ENUM::CIRCLE_GAUGE;
	circleGauge_.angle			= (float)time_ / TIME * MAX_PERCENT;

	// �I���ʒu�j������
	handEnd_.Init(systems, LAYER + 2, static_cast<int>(Texture::Game::TIMER_UI));
	handEnd_.position			= circleGauge_.position;
	handEnd_.position.x		   += ADJUST_HANDEND_POSITION;
	handEnd_.size				= HAND_SIZE;
	handEnd_.position.y		   -= Half(handEnd_.size.y * SCALE);
	handEnd_.scale				= VECTOR2(SCALE);
	handEnd_.color				= COLOR(1, 1, 1, ALPHA);
	handEnd_.scaleOffset		= VECTOR2(Half(handEnd_.size));
	handEnd_.rotationOffset		= VECTOR2(Half(handEnd_.size.x), handEnd_.size.y + (Half(handEnd_.size.y * SCALE - handEnd_.size.y)));
	handEnd_.split				= HAND_SPLIT;
	handEnd_.pattern			= HANDEND_PATTERN;


	// ���݈ʒu�j������
	handNow_.Init(systems, LAYER + 1, (int)Texture::Game::TIMER_UI);
	handNow_.position			= circleGauge_.position;
	handNow_.position.x		   += ADJUST_HANDNOW_POSITION;
	handNow_.size				= HAND_SIZE;
	handNow_.position.y		   -= Half(handNow_.size.y * SCALE);
	handNow_.scale				= VECTOR2(SCALE);
	handNow_.angle				= circleGauge_.angle * MOVE;
	handNow_.color				= COLOR(1, 0, 0, ALPHA);
	handNow_.scaleOffset		= VECTOR2(Half(handNow_.size));
	handNow_.rotationOffset		= VECTOR2(Half(handNow_.size.x), handNow_.size.y + (Half(handNow_.size.y * SCALE - handNow_.size.y)));
	handNow_.split				= HAND_SPLIT;
	handNow_.pattern			= HANDNOW_PATTERN;

	// ���v����
	clip_.Init(Systems::Instance(), LAYER + 4, (int)Texture::Game::TIMER_UI);
	clip_.position				= transform_.position;
	clip_.position.x		   += ADJUST_CLIP_POSITION;
	clip_.size					= CLIP_SIZE;
	clip_.scale					= VECTOR2(SCALE);
	clip_.color					= COLOR(1, 1, 1, 1 * ALPHA);
	clip_.scaleOffset			= VECTOR2(Half(back_.size));
	clip_.split					= HAND_SPLIT;
	clip_.pattern				= CLIP_PATTERN;
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�				*/
void Timer::Uninit(void)
{
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	���Ԃ̌v���A�`��̍X�V		*/
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
			// �p�x��ύX
			circleGauge_.angle = (float)time_ / TIME * MAX_PERCENT;

			// �j����
			handNow_.angle = max(0, (circleGauge_.angle * MOVE));
			// ���𒴂������ԏ�ɕ`��
			if (handNow_.angle < PI) { handNow_.SetPriority(LAYER + 3); }

			// �c�菭���ŐԂ�����
			if (circleGauge_.angle < RED_TIME)
			{
				circleGauge_.color = COLOR(1, 0, 0, 0.5f * ALPHA);
			}
		}
	}
}
