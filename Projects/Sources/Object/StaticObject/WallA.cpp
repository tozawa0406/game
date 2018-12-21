#include "WallA.h"

//! @def	初期位置
static const VECTOR3 POSITION = VECTOR3(0, 3, 100);
//! @def	初期回転
static const VECTOR3 ROTATION = VECTOR3(0, 3.14f, 0);
//! @def	初期スケール
static const VECTOR3 SCALE = VECTOR3(10, 5, 5);

//! @def	メッシュの調整位置
static constexpr int ADJUST_MESH_POSITION_1 = -4;
static constexpr int ADJUST_MESH_POSITION_2 =  6;

//! @def	当たり判定のオフセット位置
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, 0, 11);
//! @def	当たり判定の法線
static const VECTOR3 COLLIDER_NORMAL = VECTOR3(0, 0, 1);

WallA::WallA(const VECTOR3& position, const VECTOR3& rotation) : Object(ObjectTag::STATIC)
	, collider_(nullptr)
{
	transform_ = Transform(position, rotation, SCALE);
	for (auto& t : transformMesh_) { t = Transform(VECTOR3(0), VECTOR3(0), VECTOR3(1)); }
}

WallA::~WallA(void)
{
}

void WallA::Init(void)
{	
	float adjust1 = ADJUST_MESH_POSITION_1;
	float adjust2 = ADJUST_MESH_POSITION_2;

	if (transform_.rotation.y == 3.14f)
	{
		adjust2 *= 1.4f;
		adjust1 *= 1.4f;
	}

	transformMesh_[0].position.x = adjust1;
	transformMesh_[1].position.x = adjust2;

	for (int i = 0; i < MESH_NUM; ++i)
	{
		transformMesh_[i].parent = &transform_;
		mesh_[i].Init(Systems::Instance(), (int)Model::Game::WALL_ROCKS_6, &transformMesh_[i]);
	}
	transform_.position.y -= 5;

	collider_ = new Collider3D::Plane(this);
	if (collider_)
	{
		MATRIX temp = MATRIX().Identity().Translation(COLLIDER_OFFSET).Rotation(transform_.rotation);
		collider_->SetOffsetPosition(VECTOR3(temp._41, temp._42, temp._43 ));
		collider_->SetNormal(COLLIDER_NORMAL);
	}
}

void WallA::Uninit(void)
{
	DeletePtr(collider_);
}
