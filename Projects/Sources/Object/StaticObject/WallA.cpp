#include "WallA.h"

//! @def	�����ʒu
static const VECTOR3 POSITION = VECTOR3(0, 3, 100);
//! @def	������]
static const VECTOR3 ROTATION = VECTOR3(0, 3.14f, 0);
//! @def	�����X�P�[��
static const VECTOR3 SCALE = VECTOR3(10, 5, 1);

//! @def	���b�V���̒����ʒu
static constexpr int ADJUST_MESH_POSITION = 5;

//! @def	�����蔻��̃I�t�Z�b�g�ʒu
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, 0, 10);
//! @def	�����蔻��̖@��
static const VECTOR3 COLLIDER_NORMAL = VECTOR3(0, 0, 1);

/* @brief	�R���X�g���N�^			*/
WallA::WallA(const VECTOR3& position, const VECTOR3& rotation) : Object(Object::Tag::STATIC)
	, collider_(nullptr)
{
	transform_ = Transform(position, rotation, SCALE);
	for (auto& t : transformMesh_) { t = Transform(VECTOR3(0), VECTOR3(0), VECTOR3(1)); }
}

/* @brief	�f�X�g���N�^			*/
WallA::~WallA(void)
{
}

/* @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void WallA::Init(void)
{
	transformMesh_[0].position.x = -ADJUST_MESH_POSITION;
	transformMesh_[1].position.x =  ADJUST_MESH_POSITION;

	for (int i = 0; i < MESH_NUM; ++i)
	{
		transformMesh_[i].parent = &transform_;
		// �e���ςɂȂ�̂ō��̂Ƃ���͕`��Ȃ���
//		mesh_[i].Init(Systems::Instance(), (int)Model::Game::WALL_ROCKS_14, &transformMesh_[i]);
	}

	collider_ = new Collider3D::Plane(this);
	if (collider_)
	{
		collider_->SetOffsetPosition(COLLIDER_OFFSET);
		collider_->SetNormal(COLLIDER_NORMAL);
	}
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void WallA::Uninit(void)
{
	DeletePtr(collider_);
}
