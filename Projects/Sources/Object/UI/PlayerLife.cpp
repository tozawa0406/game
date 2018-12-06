#include "PlayerLife.h"
#include <FrameWork/Windows/Windows.h>

//! @def	描画順
static constexpr int PRIORITY		= 100;
//! @def	描画位置
static const	 VECTOR2 POSITION	= VECTOR2(580, (float)Windows::HEIGHT * 0.1f - 30);
//! @def	描画サイズ
static const	 VECTOR2 SIZE_DRAW	= VECTOR2(800, 80);
//! @def	分割数
static const	 VECTOR2 SPLIT		= VECTOR2(1, 3);

//! @def	バーのサイズ
static const	 VECTOR2 SIZE_BAR	= VECTOR2(SIZE_DRAW.x - 5, SIZE_DRAW.y * 0.8f);
//! @def	ライフバー
static const	 VECTOR2 LIFE_POSITION		= VECTOR2(POSITION.x - 5, POSITION.y - 6.7f);
//! @def	スタミナバー
static const	 VECTOR2 STAMINA_POSITION	= VECTOR2(POSITION.x - 5, POSITION.y + 10);

//! @def	ライフバー減少量
static const	 float ONE_LIFE  = SIZE_BAR.x / 150;
//! @def	バーのサイズと位置の比率
static const     float ONE_RANGE = 355.0f / 150.0f;

/* @brief	コンストラクタ			*/
PlayerLife::PlayerLife(void) : Object(Object::Tag::UI)
	, player_(nullptr)
{
}

/* @brief	デストラクタ			*/
PlayerLife::~PlayerLife(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	なし					*/
void PlayerLife::Init(void)
{
	const auto& systems = Systems::Instance();
	int texNum = static_cast<int>(Texture::Game::HP_BAR);

	// 背景
	back_.Init(systems, PRIORITY, texNum);
	back_.position	= POSITION;
	back_.size		= SIZE_DRAW;
	back_.split		= SPLIT;

	// ライフ
	life_.Init(systems, PRIORITY + 1, texNum);
	life_.position	= LIFE_POSITION;
	life_.size		= SIZE_BAR;
	life_.split		= SPLIT;
	life_.pattern	= 1;

	// スタミナ
	stamina_.Init(systems, PRIORITY + 1, texNum);
	stamina_.position	= STAMINA_POSITION;
	stamina_.size		= SIZE_BAR;
	stamina_.split		= SPLIT;
	stamina_.pattern	= 2;
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void PlayerLife::Uninit(void)
{
}

/* @brief	更新処理
 * @param	なし
 * @return	なし					*/
void PlayerLife::Update(void)
{
	if (!player_) { return; }

	int life = player_->GetLife();
	life_.size.x		= life * ONE_LIFE;
	life_.position.x	= LIFE_POSITION.x - ((150 - life) * ONE_RANGE);

	float stamina = player_->GetStamina();
	stamina_.size.x		= stamina * ONE_LIFE;
	stamina_.position.x = LIFE_POSITION.x - ((150 - stamina) * ONE_RANGE);

}
