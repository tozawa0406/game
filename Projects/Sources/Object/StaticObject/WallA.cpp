#include "WallA.h"

//! @def	初期位置
static const VECTOR3 POSITION = VECTOR3(0, 3, 100);
//! @def	初期回転
static const VECTOR3 ROTATION = VECTOR3(0, 3.14f, 0);
//! @def	初期スケール
static const VECTOR3 SCALE = VECTOR3(10, 5, 1);

//! @def	色
static const COLOR MESH_COLOR = COLOR(1, 1, 1, 1);

/* @fn		コンストラクタ
 * @brief	変数の初期化				*/
WallA::WallA(const VECTOR3& position, const VECTOR3& rotation) : Object(Object::Tag::STATIC)
	, collider_(nullptr)
{
	transform_ = Transform(position, rotation, SCALE);
}

/* @fn		デストラクタ
 * @brief	...							*/
WallA::~WallA(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし
 * @detail	姿勢、メッシュ、当たり判定の初期化	　*/
void WallA::Init(void)
{
	transformMesh_[0].parent = &transform_;
	transformMesh_[1].parent = &transform_;

	transformMesh_[0].position.x = -5;
	transformMesh_[1].position.x =  5;

	for (int i = 0; i < 2; ++i)
	{
//		mesh_[i].Init(Systems::Instance(), (int)Model::Game::WALL_ROCKS_14, &transformMesh_[i]);
	}

	collider_ = new Collider3D::Plane(this);
	if (collider_)
	{
		collider_->SetOffsetPosition(VECTOR3(0, 0, 10));
		collider_->SetNormal(VECTOR3(0, 0, 1));
	}
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし						*/
void WallA::Uninit(void)
{
	DeletePtr(collider_);
}
