#include "Timer.h"

// デバッグ用
#ifdef _DEBUG
// 時間の計測を秒単位に変更する
#define TIME_TO_SECOND
#endif

//! @def	初期位置
static const	 VECTOR2 POSITION	= VECTOR2(Graphics::WIDTH * 0.1f, Graphics::HEIGHT * 0.1f);
//! @def	描画順
static constexpr uint8 LAYER		= 100;
//! @def	全体のスケール
static constexpr float SCALE		= 0.8f;
//! @def	全体の透明度
static constexpr uint8 ALPHA		= 1;
//! @def	時計の最大は60
static constexpr uint8 TIME			= 60;
//! @def	最初の時間
static constexpr uint8 START_TIME	= 50;
//! @def	最大割合
static constexpr int   MAX_PERCENT	= 100;
//! @def	動く量
static constexpr float MOVE			= 0.0628f;
//! @def	赤くなるタイミング
static constexpr int   RED_TIME		= 10;

//! @def	背景の位置調整
static constexpr uint8 ADJUST_BACK_POSITION = 10;
//! @def	背景の大きさ
static constexpr float BACK_SIZE	= 150;
//! @def	背景の分割数
static const	 VECTOR2 BACK_SPLIT = VECTOR2(2, 1);

//! @def	ゲージの大きさ
static constexpr float   GAUGE_SIZE	= 100;

//! @def	針画像の分割数
static const	 VECTOR2 HAND_SPLIT = VECTOR2(6, 1);
//! @def	針の大きさ
static const	 VECTOR2 HAND_SIZE	= VECTOR2(40, 65);

//! @def	終了針の位置調整
static constexpr float ADJUST_HANDEND_POSITION = -1;
//! @def	終了針のテクスチャ位置
static constexpr uint8	 HANDEND_PATTERN	= 3;

//! @def	現在針の位置調整
static constexpr float	 ADJUST_HANDNOW_POSITION = -0.85f;
//! @def	終了針のテクスチャ位置
static constexpr uint8	 HANDNOW_PATTERN = 4;

//! @def	時計中央留め具位置調整
static constexpr float	 ADJUST_CLIP_POSITION = -11.5f;
//! @def	時計中央留め具の大きさ
static const	 VECTOR2 CLIP_SIZE = VECTOR2(50, 150);
//! @def	時計中央留め具のテクスチャ位置
static constexpr uint8	 CLIP_PATTERN = 5;



/* @fn		コンストラクタ
 * @brief	変数の初期化		*/
Timer::Timer(void) : Object(Object::Tag::UI), frame_(0), time_(START_TIME), second_(0)
{
}

/* @fn		デストラクタ
 * @brief	...					*/
Timer::~Timer(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし				*/
void Timer::Init(void)
{
	// 位置を指定
	transform_.position = POSITION;

	const auto& systems = Systems::Instance();

	// 背景
	back_.Init(systems, LAYER - 1, static_cast<int>(Texture::Game::TIMER_UI));
	back_.position				= transform_.position;
	back_.position.x		   -= ADJUST_BACK_POSITION;
	back_.size					= VECTOR2(BACK_SIZE);
	back_.color					= COLOR(1, 1, 1, 1 * ALPHA);
	back_.scale					= VECTOR2(SCALE);
	back_.scaleOffset			= VECTOR2(Half(back_.size));
	back_.split					= BACK_SPLIT;
	back_.pattern				= 0;

	// ゲージの初期化
	circleGauge_.Init(systems, LAYER, static_cast<int>(Texture::Base::WHITE));
	circleGauge_.position		= transform_.position;
	circleGauge_.size			= VECTOR2(GAUGE_SIZE);
	circleGauge_.color			= COLOR(1, 1, 1, 0.5f * ALPHA);
	circleGauge_.scale			= VECTOR2(SCALE);
	circleGauge_.scaleOffset	= VECTOR2(Half(circleGauge_.size));
	circleGauge_.shader			= Shader::ENUM::CIRCLE_GAUGE;
	circleGauge_.angle			= (float)time_ / TIME * MAX_PERCENT;

	// 終了位置針初期化
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


	// 現在位置針初期化
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

	// 時計中央
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
 * @brief	後処理
 * @param	なし
 * @return	なし				*/
void Timer::Uninit(void)
{
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	なし
 * @detail	時間の計測、描画の更新		*/
void Timer::Update(void)
{
#ifdef TIME_TO_SECOND
#else
	frame_++;
	// 一秒経過
	if (frame_ > TIME)
#endif
	{
		frame_ = 0;
		second_++;
		if (second_ > TIME)
		{
			second_ = 0;
			time_--;
			// 角度を変更
			circleGauge_.angle = (float)time_ / TIME * MAX_PERCENT;

			// 針を回す
			handNow_.angle = max(0, (circleGauge_.angle * MOVE));
			// 一定を超えたら一番上に描画
			if (handNow_.angle < PI) { handNow_.SetPriority(LAYER + 3); }

			// 残り少しで赤くする
			if (circleGauge_.angle < RED_TIME)
			{
				circleGauge_.color = COLOR(1, 0, 0, 0.5f * ALPHA);
			}
		}
	}
}
