#include "TutorialBase.h"

const VECTOR2	TutorialBase::TEXT_POSITION		= VECTOR2(Windows::WIDTH - 380, 140);
const VECTOR2	TutorialBase::TEXT_DRAW_SIZE	= VECTOR2(200, 40);
const COLOR		TutorialBase::COLOR_PUSH  = COLOR(1, 0, 0.3f, 1);
const COLOR		TutorialBase::COLOR_WAIT  = COLOR(1, 0.6f, 0.6f, 1);
const COLOR		TutorialBase::COLOR_WHITE = COLOR(1);

//! @def	描画順
static constexpr	uint8 PRIORITY = 210;

//! @def	ゲージの大きさ
static constexpr	float GAUGE_SIZE		= 45;
//! @def	ゲージ背景の大きさ
static constexpr	float GAUGE_BACK_SIZE	= 50;
//! @def	ゲージのY位置のずらし量
static constexpr	float GAUGE_POSITION_Y	= 80;

//! @def	ゲージの色
static const		COLOR GAUGE_COLOR			= COLOR::RGBA(221, 238, 255);
//! @def	成功時のゲージ点滅色
static const		COLOR GAUGE_SUCCESS_COLOR	= COLOR(1, 0.3f, 0.3f);
//! @def	ゲージ背景の色
static const		COLOR GAUGE_BACK_COLOR		= COLOR(0.3f, 0.3f, 0.3f);

//! @def	最大割合
static constexpr	float	MAX_PERCENT		= 100;
//! @def	成功時の点滅の間隔
static constexpr	int		FLASHING_RANGE	= 10;

TutorialBase::TutorialBase(void) : 
	maxCnt_(0)
	, cnt_(0)
	, ctrl_(nullptr) 
{
}

TutorialBase::~TutorialBase(void)
{
}

void TutorialBase::Init(TutorialManager* manager, Controller* ctrl)
{
	manager_	= manager; 
	ctrl_		= ctrl; 

	// 共通変数
	int texNum	= static_cast<int>(Resources::Texture::Base::WHITE);
	VECTOR2 pos = TutorialManager::POSITION;
	pos.y += GAUGE_POSITION_Y;

	// ゲージ
	gauge_.Init(PRIORITY, texNum);
	gauge_.SetSize(VECTOR2(GAUGE_SIZE));
	gauge_.SetPosition(pos);
	gauge_.SetColor(GAUGE_COLOR);
	gauge_.SetShader(Shader::ENUM::CIRCLE_GAUGE);
	gauge_.SetAngle(0);

	// 背景
	gaugeBack_.Init(PRIORITY - 1, texNum);
	gaugeBack_.SetSize(VECTOR2(GAUGE_BACK_SIZE));
	gaugeBack_.SetPosition(pos);
	gaugeBack_.SetColor(GAUGE_BACK_COLOR);
	gaugeBack_.SetShader(Shader::ENUM::CIRCLE_GAUGE);
	gaugeBack_.SetAngle(MAX_PERCENT);
}

void TutorialBase::Uninit(void)
{
	text_.Uninit();
	gauge_.Uninit();
	gaugeBack_.Uninit();
}

void TutorialBase::UpdateTimer(void)
{
	// クリアしたらクリア演出
	if (cnt_ >= maxCnt_) { cnt_ = -1; }

	// クリアしたら
	if (cnt_ < 0)
	{
		// 点滅
		cnt_--;
		gauge_.SetAngle(MAX_PERCENT);
		if (cnt_ % (FLASHING_RANGE * 2) < -FLASHING_RANGE)
		{
			gauge_.SetColor(GAUGE_COLOR);
		}
		else
		{
			gauge_.SetColor(GAUGE_SUCCESS_COLOR);
		}
	}
	// 進行中
	else
	{
		float a = static_cast<float>(cnt_) / maxCnt_ * MAX_PERCENT;
		gauge_.SetAngle(a);
	}
}
