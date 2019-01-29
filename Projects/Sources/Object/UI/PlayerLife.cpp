#include "PlayerLife.h"
#include "../GameObject/Player/Player.h"
#include <FrameWork/Windows/Windows.h>

//! @def	描画順
static constexpr int PRIORITY		= 100;
//! @def	描画位置
static const	 VECTOR2 POSITION	= VECTOR2(580, Windows::HEIGHT * 0.1f - 30);
//! @def	ライフ背景の描画位置
static const	 VECTOR2 LIFE_BACK_POSITION = VECTOR2(POSITION.x, POSITION.y - 6.7f);
//! @def	スタミナ背景の描画位置
static const	 VECTOR2 STAMINA_BACK_POSITION = VECTOR2(POSITION.x, POSITION.y + 10);
//! @def	値に対する重み(横方向)
static constexpr float	 X_WEIGHT	= 5;
//! @def	描画サイズ
static const	 VECTOR2 SIZE_DRAW	= VECTOR2(750, 20);
//! @def	分割数
static const	 VECTOR2 SPLIT		= VECTOR2(1, 5);

//! @def	バーのサイズ
static const	 VECTOR2 SIZE_BAR	= VECTOR2(SIZE_DRAW.x - 5, SIZE_DRAW.y * 0.8f);
//! @def	ライフバー
static const	 VECTOR2 LIFE_POSITION		= VECTOR2(POSITION.x - 5, POSITION.y - 6.7f);
//! @def	スタミナバー
static const	 VECTOR2 STAMINA_POSITION	= VECTOR2(POSITION.x - 5, POSITION.y + 10);
//! @def	装飾のサイズ
static constexpr float SIZE_ORNAMENT = 20;
//! @def	装飾の画像サイズ
static constexpr float SIZE_TEX_ORNAMENT = 32;

//! @def	ライフバー背景減少量
static const	 float ONE_LIFE_BACK = SIZE_DRAW.x / PlayerLife::MAX_LIFE;
//! @def	ライフバー減少量
static const	 float ONE_LIFE  = SIZE_BAR.x / PlayerLife::MAX_LIFE;
//! @def	バーのサイズと位置の比率
static const     float ONE_RANGE = 340 / PlayerLife::MAX_LIFE;

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
	backLife_.SetPattern(1);

	// スタミナ背景
	backStamina_.Init(PRIORITY, texNum);
	backStamina_.SetPosition(STAMINA_BACK_POSITION);
	backStamina_.SetSize(SIZE_DRAW);
	backStamina_.SetSplit(SPLIT);
	backStamina_.SetPattern(2);

	// ライフ
	life_.Init(PRIORITY + 1, texNum);
	life_.SetPosition(LIFE_POSITION);
	life_.SetSize(SIZE_BAR);
	life_.SetSplit(SPLIT);
	life_.SetPattern(3);

	// スタミナ
	stamina_.Init(PRIORITY + 1, texNum);
	stamina_.SetPosition(STAMINA_POSITION);
	stamina_.SetSize(SIZE_BAR);
	stamina_.SetSplit(SPLIT);
	stamina_.SetPattern(4);

	// 両端の装飾
	for (int i = 0;i < 4;++i)
	{
		ornament_[i].Init(PRIORITY + 2, texNum);
		ornament_[i].SetSize(VECTOR2(SIZE_ORNAMENT - (i / 2)));
		ornament_[i].SetSplit(VECTOR2(16, SPLIT.y));
		ornament_[i].SetPattern(static_cast<float>(i));
	}

	float pos = POSITION.x - Half(SIZE_DRAW.x) + Half(SIZE_TEX_ORNAMENT) + 10;
	ornament_[0].SetPosition(VECTOR2(pos, LIFE_BACK_POSITION.y));
	ornament_[1].SetPosition(VECTOR2(pos, STAMINA_BACK_POSITION.y));

	pos = POSITION.x + Half(SIZE_DRAW.x) - SIZE_TEX_ORNAMENT - 1;
	ornament_[2].SetPosition(VECTOR2(pos, LIFE_BACK_POSITION.y));
	ornament_[3].SetPosition(VECTOR2(pos, STAMINA_BACK_POSITION.y));

}

void PlayerLife::Uninit(void)
{
	stamina_.Uninit();
	life_.Uninit();
	backStamina_.Uninit();
	backLife_.Uninit();
	for (auto& o : ornament_)
	{
		o.Uninit();
	}
}

void PlayerLife::Update(void)
{
	if (!player_) { return; }
	const auto& max = player_->GetMaxLifeStamina();

	// ライフ背景
	MoveBar(backLife_, max.x, LIFE_BACK_POSITION, ONE_LIFE_BACK, &ornament_[2]);
	// スタミナ背景
	MoveBar(backStamina_, max.y, STAMINA_BACK_POSITION, ONE_LIFE_BACK, &ornament_[3]);	
	// ライフバー
	MoveBar(life_, static_cast<float>(player_->GetLife()), LIFE_POSITION, ONE_LIFE, nullptr);
	// スタミナバー
	MoveBar(stamina_, player_->GetStamina(), STAMINA_POSITION, ONE_LIFE, nullptr);
}

void PlayerLife::MoveBar(CanvasRenderer::Image& bar, float value, const VECTOR2& offset, const float weight, CanvasRenderer::Image* ornament)
{
	// 現在値取得
	auto pos	= bar.GetPosition();
	auto size	= bar.GetSize();

	// 値に応じてサイズ及び位置を変更
	pos.x	= offset.x - ((MAX_LIFE - value) * ONE_RANGE);
	size.x	= value * weight;

	if (ornament)
	{
		float diff = value / MAX_LIFE;
		if (value < 100) { diff *= value / 100; }
		float position = pos.x + Half(size.x) - SIZE_TEX_ORNAMENT * diff;
		ornament->SetPosition(VECTOR2(position, offset.y));
	}


	// 変更値を設定
	bar.SetPosition(pos);
	bar.SetSize(size);
}
