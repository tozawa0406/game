#include "WallParts.h"

//! @def	メッシュの調整位置
static constexpr int ADJUST_MESH_POSITION_1 = -4;
static constexpr int ADJUST_MESH_POSITION_2 = 6;

//! @def	初期スケール
static const VECTOR3 SCALE = VECTOR3(10, 5, 5);

//! @def	当たり判定のオフセット位置
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, 0, 11);
//! @def	当たり判定の法線
static const VECTOR3 COLLIDER_NORMAL = VECTOR3(0, 0, 1);

WallParts::WallParts(const VECTOR3& position, const VECTOR3& rotation) : Object(ObjectTag::STATIC)
	, collider_(nullptr)
{
	transform_ = Transform(position, rotation, SCALE);
	for (auto& t : transformMesh_) { t = Transform(VECTOR3(0), VECTOR3(0), VECTOR3(1)); }
}

WallParts::~WallParts(void)
{
}

void WallParts::Init(void)
{	
	transformMesh_[0].position.x = ADJUST_MESH_POSITION_1;
	transformMesh_[1].position.x = ADJUST_MESH_POSITION_2;

	for (int i = 0; i < MESH_NUM; ++i)
	{
		transformMesh_[i].parent = &transform_;
		mesh_[i].Init(Systems::Instance(), (int)Resources::Model::Camp::ROCK_6, &transformMesh_[i]);
	}
	transform_.position.y -= 5;

	collider_ = new Collider3D::Plane(this);
	if (collider_)
	{
		MATRIX temp = MATRIX().Identity().Translation(COLLIDER_OFFSET).Rotation(transform_.rotation);
		collider_->SetOffsetPosition(VECTOR3(temp._41, temp._42, temp._43));
		collider_->SetNormal(COLLIDER_NORMAL);
	}
}

void WallParts::Uninit(void)
{
	DeletePtr(collider_);
}
