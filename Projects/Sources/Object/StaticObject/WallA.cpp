#include "WallA.h"

//! @def	�����ʒu
static const VECTOR3 POSITION = VECTOR3(0, 3, 100);
//! @def	������]
static const VECTOR3 ROTATION = VECTOR3(0, 3.14f, 0);
//! @def	�����X�P�[��
static const VECTOR3 SCALE = VECTOR3(10, 5, 1);

//! @def	�F
static const COLOR MESH_COLOR = COLOR(1, 1, 1, 1);

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����				*/
WallA::WallA(const VECTOR3& position, const VECTOR3& rotation) : Object(Object::Tag::STATIC)
	, collider_(nullptr)
{
	transform_ = Transform(position, rotation, SCALE);
}

/* @fn		�f�X�g���N�^
 * @brief	...							*/
WallA::~WallA(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�p���A���b�V���A�����蔻��̏�����	�@*/
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
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�						*/
void WallA::Uninit(void)
{
	DeletePtr(collider_);
}
