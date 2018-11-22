#include "BloodSplash.h"

/* @fn		コンストラクタ
 * @brief	初期化				*/
BloodSplash::BloodSplash(VECTOR3 position) : Object(Object::Tag::BILLBOARD)
{
	transform_.position = position;
	transform_.scale = VECTOR3(10);
}

/* @fn		デストラクタ
 * @brief						*/
BloodSplash::~BloodSplash(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし				*/
void BloodSplash::Init(void)
{
	renderer_.split = VECTOR2(8, 2);
	renderer_.Init(Systems::Instance(), static_cast<int>(Texture::Game::EXPLOSION), &transform_);
	renderer_.billbord	= true;
	renderer_.shadow	= false;
	renderer_.shader	= Shader::ENUM::BILLBOARD;
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし				*/
void BloodSplash::Uninit(void)
{
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	なし				*/
void BloodSplash::Update(void)
{
	if (renderer_.Animation(0.5f))
	{
		Destroy();
	}
}
