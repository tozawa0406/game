#include "PaidGoodsBox.h"

//! @def	初期位置
static const VECTOR3 POSITION = VECTOR3(-30, 9.5f, 55);
//! @def	初期回転
static const VECTOR3 ROTATION = VECTOR3(0, -1.3f, 0);
//! @def	初期スケール
static const VECTOR3 SCALE = VECTOR3(0.05f, 0.05f * 0.8f, 0.05f);

//! @def	色
static const COLOR MESH_COLOR = COLOR(0.25f, 0.25f, 1, 1);

//! @def	当たり判定のサイズ
static const VECTOR3 COLLIDER_SIZE = VECTOR3(15, 12.5f, 31);
//! @def	当たり判定のオフセット
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, -2, 1);



/* @fn		コンストラクタ
 * @brief	変数の初期化				*/
PaidGoodsBox::PaidGoodsBox(void) : Object(Object::Tag::STATIC)
{
}

/* @fn		デストラクタ
 * @brief	...							*/
PaidGoodsBox::~PaidGoodsBox(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし
 * @detail	姿勢、メッシュ、当たり判定の初期化	　*/
void PaidGoodsBox::Init(void)
{
	// 初期姿勢の設定
	transform_ = Transform(POSITION, ROTATION, SCALE);

	// メッシュ
	mesh_.Init(Systems::Instance(), (int)Model::Game::BOX, &transform_);
	mesh_.material.diffuse = MESH_COLOR;

	// 当たり判定
	collider_ = new Collider3D::OBB(this);
	if (collider_)
	{
		collider_->SetSize(COLLIDER_SIZE);
		collider_->SetOffset(COLLIDER_OFFSET);
		collider_->Update();
	}
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし						*/
void PaidGoodsBox::Uninit(void)
{
}
