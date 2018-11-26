#include "BloodSplash.h"

/* @fn		コンストラクタ
 * @brief	初期化				*/
BloodSplash::BloodSplash(VECTOR3 position, float rotation) : Object(Object::Tag::BILLBOARD)
	, cnt_(0)
{
	transform_.position		= position;
	transform_.scale		= VECTOR3(2, 10, 1);
	transform_.rotation.z	= rotation;
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
	renderer_.Init(Systems::Instance(), static_cast<int>(Texture::Game::SLASH), &transform_);
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
	cnt_++;
	if (cnt_ > 30)
	{
		Destroy();
	}
}
