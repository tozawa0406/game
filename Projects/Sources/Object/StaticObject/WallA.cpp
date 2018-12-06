#include "WallA.h"

//! @def	初期位置
static const VECTOR3 POSITION = VECTOR3(0, 3, 100);
//! @def	初期回転
static const VECTOR3 ROTATION = VECTOR3(0, 3.14f, 0);
//! @def	初期スケール
static const VECTOR3 SCALE = VECTOR3(10, 5, 1);

//! @def	メッシュの調整位置
static constexpr int ADJUST_MESH_POSITION = 5;

//! @def	当たり判定のオフセット位置
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, 0, 10);
//! @def	当たり判定の法線
static const VECTOR3 COLLIDER_NORMAL = VECTOR3(0, 0, 1);

/* @brief	コンストラクタ			*/
WallA::WallA(const VECTOR3& position, const VECTOR3& rotation) : Object(Object::Tag::STATIC)
	, collider_(nullptr)
{
	transform_ = Transform(position, rotation, SCALE);
	for (auto& t : transformMesh_) { t = Transform(VECTOR3(0), VECTOR3(0), VECTOR3(1)); }
}

/* @brief	デストラクタ			*/
WallA::~WallA(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	なし					*/
void WallA::Init(void)
{
	transformMesh_[0].position.x = -ADJUST_MESH_POSITION;
	transformMesh_[1].position.x =  ADJUST_MESH_POSITION;

	for (int i = 0; i < MESH_NUM; ++i)
	{
		transformMesh_[i].parent = &transform_;
		// 影が変になるので今のところは描画なしで
//		mesh_[i].Init(Systems::Instance(), (int)Model::Game::WALL_ROCKS_14, &transformMesh_[i]);
	}

	collider_ = new Collider3D::Plane(this);
	if (collider_)
	{
		collider_->SetOffsetPosition(COLLIDER_OFFSET);
		collider_->SetNormal(COLLIDER_NORMAL);
	}
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void WallA::Uninit(void)
{
	DeletePtr(collider_);
}
