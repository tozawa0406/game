#include "WallA.h"

//! @def	初期スケール
static const VECTOR3 SCALE = VECTOR3(10, 5, 5);

//! @def	メッシュの調整位置
static constexpr int ADJUST_MESH_POSITION_1 = -4;
static constexpr int ADJUST_MESH_POSITION_2 =  6;

//! @def	当たり判定のオフセット位置
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, 0, 11);
//! @def	当たり判定の法線
static const VECTOR3 COLLIDER_NORMAL = VECTOR3(0, 0, 1);
//! @def	入口の当たり判定のオフセット調整位置1
static constexpr float ENTRANCE_POSITION_ADJUST1 = 9.5f;
//! @def	入口の当たり判定のオフセット調整位置2
static constexpr float ENTRANCE_POSITION_ADJUST2 = 2;
//! @def	入口の当たり判定の大きさ
static const VECTOR3 ENTRANCE_COLLIDER_SIZE = VECTOR3(100, 80, 25);

WallA::WallA(const VECTOR3& position, const VECTOR3& rotation) : Object(ObjectTag::STATIC)
	, collider_(nullptr)
{
	transform_ = Transform(position, rotation, SCALE);
	for (auto& t : transformMesh_) { t = Transform(VECTOR3(0), VECTOR3(0), VECTOR3(1)); }
	for (auto& c : entranceCollider_) { c = nullptr; }
}

WallA::~WallA(void)
{
}

void WallA::Init(void)
{	
	bool entrance = false;
	float adjust1 = ADJUST_MESH_POSITION_1;
	float adjust2 = ADJUST_MESH_POSITION_2;

	if (transform_.rotation.y == 3.14f)
	{
		entrance = true;
		adjust2 *= 1.4f;
		adjust1 *= 1.4f;
	}

	transformMesh_[0].position.x = adjust1;
	transformMesh_[1].position.x = adjust2;

	for (int i = 0; i < MESH_NUM; ++i)
	{
		transformMesh_[i].parent = &transform_;
		mesh_[i].Init(Systems::Instance(), (int)Model::Game::ROCK_6, &transformMesh_[i]);
	}
	transform_.position.y -= 5;

	if (entrance)
	{
		for (int i = 0; i < 2; ++i)
		{
			entranceCollider_[i] = new Collider3D::OBB(this);
			if (entranceCollider_[i])
			{
				entranceCollider_[i]->SetOffsetPosition(transformMesh_[i].position * (ENTRANCE_POSITION_ADJUST1 - (i * ENTRANCE_POSITION_ADJUST2)));
				entranceCollider_[i]->SetSize(ENTRANCE_COLLIDER_SIZE);
			}
		}
	}
	else
	{
		collider_ = new Collider3D::Plane(this);
		if (collider_)
		{
			MATRIX temp = MATRIX().Identity().Translation(COLLIDER_OFFSET).Rotation(transform_.rotation);
			collider_->SetOffsetPosition(VECTOR3(temp._41, temp._42, temp._43));
			collider_->SetNormal(COLLIDER_NORMAL);
		}
	}
}

void WallA::Uninit(void)
{
	DeletePtr(collider_);
}
