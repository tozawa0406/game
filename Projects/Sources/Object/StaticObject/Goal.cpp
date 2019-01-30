#include "Goal.h"

//! @def	�����ʒu
static const VECTOR3 POSITION = VECTOR3(-4, 20, 100);
//! @def	������]
static const VECTOR3 ROTATION = VECTOR3(0);
//! @def	�����X�P�[��
static const VECTOR3 SCALE = VECTOR3(10, 10, 1);

//! @def	�����蔻��̃T�C�Y
static const VECTOR3 COLLIDER_SIZE = VECTOR3(16, 40, 10);
//! @def	�����蔻��̃I�t�Z�b�g
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, 0, 0);

Goal::Goal(void) : Object(ObjectTag::EVENT)
	, collider_(nullptr)
	, change_(false)
{
}

Goal::~Goal(void)
{
}

void Goal::Init(void)
{
	// �����p���̐ݒ�
	transform_ = Transform(POSITION, ROTATION, SCALE);

	// �����蔻��
	collider_ = new Collider3D::OBB(this);
	if (collider_)
	{
		collider_->SetSize(COLLIDER_SIZE);
		collider_->SetOffsetPosition(COLLIDER_OFFSET);
		collider_->SetColliderTag(ColliderTag::DEFENSE);
		collider_->SetTrigger(true);
		collider_->Update();
	}
}

void Goal::Uninit(void)
{
	DeletePtr(collider_);
}

void Goal::Update(void)
{
	if (collider_)
	{
		const auto& objects = collider_->Hit();
		for (auto& obj : objects)
		{
			if (obj && obj->GetTag() == ObjectTag::PLAYER) { change_ = true; }
		}
	}
}
