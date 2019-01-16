#include "Cart.h"

//! @def	�����ʒu
static const VECTOR3 POSITION = VECTOR3(75, -0.5f, 20);
//! @def	������]
static const VECTOR3 ROTATION = VECTOR3(-0.14f, 0, 0);
//! @def	�����X�P�[��
static const VECTOR3 SCALE = VECTOR3(3);

//! @def	�����蔻��̃T�C�Y
static const VECTOR3 COLLIDER_SIZE = VECTOR3(30, 30, 47.5f);
//! @def	�����蔻��̃I�t�Z�b�g
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, 0, 11);

Cart::Cart(void) : Object(ObjectTag::STATIC)
	, collider_(nullptr)
{
}

Cart::~Cart(void)
{
}

void Cart::Init(void)
{
	// �����p���̐ݒ�
	transform_ = Transform(POSITION, ROTATION, SCALE);

	// ���b�V��
	mesh_.Init(Systems::Instance(), static_cast<int>(Model::Camp::CART), &transform_);

	// �����蔻��
	collider_ = new Collider3D::OBB(this);
	if (collider_)
	{
		collider_->SetSize(COLLIDER_SIZE);
		collider_->SetOffsetPosition(COLLIDER_OFFSET);
		collider_->Update();
	}
}

void Cart::Uninit(void)
{
	DeletePtr(collider_);
}
