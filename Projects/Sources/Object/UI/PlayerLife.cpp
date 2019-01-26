#include "PlayerLife.h"
#include <FrameWork/Windows/Windows.h>

//! @def	描画順
static constexpr int PRIORITY		= 100;
//! @def	描画位置
static const	 VECTOR2 POSITION	= VECTOR2(580, Windows::HEIGHT * 0.1f - 30);
//! @def	ライフ背景の描画位置
static const	 VECTOR2 LIFE_BACK_POSITION = VECTOR2(POSITION.x, POSITION.y - 6.7f);
//! @def	スタミナ背景の描画位置
static const	 VECTOR2 STAMINA_BACK_POSITION = VECTOR2(POSITION.x - 2, POSITION.y + 10);
//! @def	値に対する重み(横方向)
static constexpr float	 X_WEIGHT	= 5;
//! @def	描画サイズ
static const	 VECTOR2 SIZE_DRAW	= VECTOR2(750, 20);
//! @def	分割数
static const	 VECTOR2 SPLIT		= VECTOR2(1, 4);

//! @def	バーのサイズ
static const	 VECTOR2 SIZE_BAR	= VECTOR2(SIZE_DRAW.x - 5, SIZE_DRAW.y * 0.8f);
//! @def	ライフバー
static const	 VECTOR2 LIFE_POSITION		= VECTOR2(POSITION.x - 6, POSITION.y - 6.7f);
//! @def	スタミナバー
static const	 VECTOR2 STAMINA_POSITION	= VECTOR2(POSITION.x - 6, POSITION.y + 10);

//! @def	ライフバー背景減少量
static const	 float ONE_LIFE_BACK = SIZE_DRAW.x / 150;
//! @def	ライフバー減少量
static const	 float ONE_LIFE  = SIZE_BAR.x / 150;
//! @def	バーのサイズと位置の比率
static const     float ONE_RANGE = 335 / 150.0f;

PlayerLife::PlayerLife(void) : Object(ObjectTag::UI)
	, player_(nullptr)
{
}

PlayerLife::~PlayerLife(void)
{
}

void PlayerLife::Init(void)
{
	int texNum = static_cast<int>(Resources::Texture::Camp::HP_BAR);

	// ライフ背景
	backLife_.Init(PRIORITY, texNum);
	backLife_.SetPosition(LIFE_BACK_POSITION);
	backLife_.SetSize(SIZE_DRAW);
	backLife_.SetSplit(SPLIT);
	backLife_.SetPattern(0);

	// スタミナ背景
	backStamina_.Init(PRIORITY, texNum);
	backStamina_.SetPosition(STAMINA_BACK_POSITION);
	backStamina_.SetSize(SIZE_DRAW);
	backStamina_.SetSplit(SPLIT);
	backStamina_.SetPattern(1);


	// ライフ
	life_.Init(PRIORITY + 1, texNum);
	life_.SetPosition(LIFE_POSITION);
	life_.SetSize(SIZE_BAR);
	life_.SetSplit(SPLIT);
	life_.SetPattern(2);

	// スタミナ
	stamina_.Init(PRIORITY + 1, texNum);
	stamina_.SetPosition(STAMINA_POSITION);
	stamina_.SetSize(SIZE_BAR);
	stamina_.SetSplit(SPLIT);
	stamina_.SetPattern(3);
}

void PlayerLife::Uninit(void)
{
	stamina_.Uninit();
	life_.Uninit();
	backStamina_.Uninit();
	backLife_.Uninit();
}

void PlayerLife::Update(void)
{
	if (!player_) { return; }

	const auto& max = player_->GetMaxLifeStamina();
	// ライフ背景
	MoveBar(backLife_, max.x, LIFE_BACK_POSITION, ONE_LIFE_BACK);
	// スタミナ背景
	MoveBar(backStamina_, max.y, STAMINA_BACK_POSITION, ONE_LIFE_BACK);
	// ライフバー
	MoveBar(life_, static_cast<float>(player_->GetLife()), LIFE_POSITION, ONE_LIFE);
	// スタミナバー
	MoveBar(stamina_, player_->GetStamina(), STAMINA_POSITION, ONE_LIFE);
}

void PlayerLife::MoveBar(CanvasRenderer::Image& bar, float value, const VECTOR2& offset, const float weight)
{
	// 現在値取得
	auto pos	= bar.GetPosition();
	auto size	= bar.GetSize();

	// 値に応じてサイズ及び位置を変更
	pos.x	= offset.x - ((150 - value) * ONE_RANGE);
	size.x	= value * weight;

	// 変更値を設定
	bar.SetPosition(pos);
	bar.SetSize(size);
}
