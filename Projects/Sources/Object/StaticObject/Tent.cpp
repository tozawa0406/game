#include "Tent.h"

//! @def	初期位置
static const VECTOR3 POSITION = VECTOR3(-18 - 40, 0.5f, -50);
//! @def	初期回転
static const VECTOR3 ROTATION = VECTOR3(0, 1.57f, 0.045f);
//! @def	初期スケール
static const VECTOR3 SCALE = VECTOR3(3);

//! @def	当たり判定のサイズ
static const VECTOR3 COLLIDER_SIZE = VECTOR3(40, 10, 26);
//! @def	当たり判定のオフセット
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(18, 5, 10);
//! @def	当たり判定のオフセット
static const VECTOR3 COLLIDER_ROTATION = VECTOR3(0, 0, -ROTATION.z * SCALE.z);

Tent::Tent(void) : Object(ObjectTag::STATIC)
	, collider_(nullptr)
{
}

Tent::~Tent(void)
{
}

void Tent::Init(void)
{
	// 初期姿勢の設定
	transform_ = Transform(POSITION, ROTATION, SCALE);

	// メッシュ
	mesh_.Init(Systems::Instance(), static_cast<int>(Model::Camp::TENT), &transform_);

	// 当たり判定
	collider_ = new Collider3D::OBB(this);
	if (collider_)
	{
		collider_->SetSize(COLLIDER_SIZE);
		collider_->SetOffsetPosition(COLLIDER_OFFSET);
		collider_->SetOffsetRotation(COLLIDER_ROTATION);
		collider_->Update();
	}
}

void Tent::Uninit(void)
{
	DeletePtr(collider_);
}
