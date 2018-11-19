#include "PlayerLife.h"
#include <FrameWork/Windows/Windows.h>

/* @fn		コンストラクタ
 * @brief	変数の初期化		*/
PlayerLife::PlayerLife(void) : Object(Object::Tag::UI)
	, player_(nullptr)
{
}

/* @fn		デストラクタ
 * @brief	...					*/
PlayerLife::~PlayerLife(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし				*/
void PlayerLife::Init(void)
{
	const auto& systems = Systems::Instance();

	back_.Init(systems, 100, static_cast<int>(Texture::Game::HP_BAR));
	back_.position = VECTOR2(580, (float)Windows::HEIGHT * 0.1f - 30);
	back_.size = VECTOR2(800, 80);
	back_.split = VECTOR2(1, 3);
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし				*/
void PlayerLife::Uninit(void)
{
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	なし				*/
void PlayerLife::Update(void)
{
	if (!player_) { return; }


}
