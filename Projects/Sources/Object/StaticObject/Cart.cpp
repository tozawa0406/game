#include "Cart.h"

//! @def	初期位置
static const VECTOR3 POSITION = VECTOR3(75, -0.5f, 20);
//! @def	初期回転
static const VECTOR3 ROTATION = VECTOR3(-0.14f, 0, 0);
//! @def	初期スケール
static const VECTOR3 SCALE = VECTOR3(3);

//! @def	当たり判定のサイズ
static const VECTOR3 COLLIDER_SIZE = VECTOR3(30, 30, 47.5f);
//! @def	当たり判定のオフセット
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, 0, 11);

Cart::Cart(void) : Object(ObjectTag::STATIC)
	, collider_(nullptr)
{
}

Cart::~Cart(void)
{
}

void Cart::Init(void)
{
	// 初期姿勢の設定
	transform_ = Transform(POSITION, ROTATION, SCALE);

	// メッシュ
	mesh_.Init(Systems::Instance(), static_cast<int>(Model::Camp::CART), &transform_);

	// 当たり判定
	collider_ = new Collider3D::OBB(this);
	if (collider_)
	{
		collider_->SetSize(COLLIDER_SIZE);
		collider_->SetOffsetPosition(COLLIDER_OFFSET);
		collider_->Update();
	}
}

void Cart::Uninit(void)
{
	DeletePtr(collider_);
}
