#include "Tent.h"

//! @def	�����ʒu
static const VECTOR3 POSITION = VECTOR3(-18 - 40, 0.5f, -50);
//! @def	������]
static const VECTOR3 ROTATION = VECTOR3(0, 1.57f, 0.045f);
//! @def	�����X�P�[��
static const VECTOR3 SCALE = VECTOR3(3);

//! @def	�����蔻��̃T�C�Y
static const VECTOR3 COLLIDER_SIZE = VECTOR3(40, 10, 26);
//! @def	�����蔻��̃I�t�Z�b�g
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(18, 5, 10);
//! @def	�����蔻��̃I�t�Z�b�g
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
	// �����p���̐ݒ�
	transform_ = Transform(POSITION, ROTATION, SCALE);

	// ���b�V��
	mesh_.Init(Systems::Instance(), static_cast<int>(Model::Camp::TENT), &transform_);

	// �����蔻��
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
