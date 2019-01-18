#include "CampWallParts.h"

//! @def	入口の当たり判定のオフセット調整位置1
static constexpr float ENTRANCE_POSITION_ADJUST1 = 9.5f;
//! @def	入口の当たり判定のオフセット調整位置2
static constexpr float ENTRANCE_POSITION_ADJUST2 = 2;
//! @def	入口の当たり判定の大きさ
static const VECTOR3 ENTRANCE_COLLIDER_SIZE = VECTOR3(100, 80, 25);

CampWallParts::CampWallParts(const VECTOR3& position, const VECTOR3& rotation) : WallParts(position, rotation)
{
	for (auto& c : entranceCollider_) { c = nullptr; }
}

CampWallParts::~CampWallParts(void)
{
}

void CampWallParts::Init(void)
{	
	WallParts::Init();

	bool entrance = false;
	if (transform_.rotation.y == 3.14f)
	{
		entrance = true;
		transformMesh_[0].position.x *= 1.4f;
		transformMesh_[1].position.x *= 1.4f;
	}

	if (entrance)
	{
		DeletePtr(collider_);
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
}

void CampWallParts::Uninit(void)
{
	WallParts::Uninit();
	for (auto& c : entranceCollider_) { DeletePtr(c); }
}
