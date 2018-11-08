//-----------------------------------------------------------------------------
//
//	3D“–‚½‚è”»’è[Collider3D.cpp]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Collider3D.h"
#include "../GameSystems.h"
#include "../../Object/Object.h"
#include "../../Object/ObjectManager.h"

Collider3DBase::Collider3DBase(Object* obj, Type type) : systems_(Systems::Instance()), transform_(obj->GetTransform())
													   , object_(obj), type_(type), enable_(true), parentMtx_(nullptr), transMtx_(nullptr)
													   , offsetPosition_(VECTOR3(0)), offsetRotation_(VECTOR3(0)), size_(VECTOR3(1))
{
	for (int i = 0; i < 3; ++i)
	{
		direction_[i] = VECTOR3(0);
	}

	systems_->GetCollision3D()->Add(this);
}

Collider3DBase::~Collider3DBase(void)
{
	systems_->GetCollision3D()->Remove(this);
}

std::vector<Object*> Collider3DBase::Hit(void)
{
	return list_;
}

Collider3D::Segment::Segment(Object* obj) : Collider3DBase(obj, Type::SEGMENT)
{
	renderer_.ColliderSegment(this);
}

void Collider3D::Segment::Update(void)
{
	transform_ = object_->GetTransform();
	transform_.position += offsetPosition_;

	renderer_.enable = false;
	if (!systems_->GetDebug()->GetDebug()) { return; }

	renderer_.Update(this);
}

Collider3D::Sphere::Sphere(Object* obj) : Collider3DBase(obj, Type::SPHERE)
{
	renderer_.ColliderSphere(this, false);
	renderer2_.ColliderSphere(this, true);
}

void Collider3D::Sphere::Update(void)
{
	transform_ = object_->GetTransform();
	transform_.position = object_->GetTransform().globalPosition;
	transform_.position += offsetPosition_;
	transform2_ = transform_;

	renderer_.enable = renderer2_.enable = IsEnable();

	if (!systems_->GetDebug()->GetDebug()) { return; }

	renderer_.Update(this);
	renderer2_.Update(this);
}

Collider3D::OBB::OBB(Object* obj) : Collider3DBase(obj, Type::OBB)
{
	for (int i = 0; i < 3; ++i)
	{
		normaDirect_[i] = VECTOR3(0);
		length_[i] = 0;
	}

	renderer_.ColliderOBB(this);
}

void Collider3D::OBB::Update(void)
{
	transform_ = object_->GetTransform();

	renderer_.enable = IsEnable();

	MATRIX m = MATRIX().Identity();

	VECTOR3 mScale = VECTOR3(1);
	if (transMtx_)
	{
		mScale = VECTOR3(1 / transMtx_->_11, 1 / transMtx_->_22, 1 / transMtx_->_33);
	}

	const auto& scale = transform_.scale;
	VECTOR3 s = VECTOR3(1 / scale.x, 1 / scale.y, 1 / scale.z);
	m.Scaling(size_ * s * mScale);
	m.Rotation(offsetRotation_ * s * mScale);
	m.Translation(offsetPosition_ * s * mScale);

	if (parentMtx_)
	{
		MATRIX temp = *parentMtx_;
		if (transMtx_)
		{
			temp *= *transMtx_;
		}
		temp.Scaling(scale);
		m *= temp;
	}

	m.Create(&transform_);

	s = VECTOR3(1);
	if (transform_.parent)
	{
		s = transform_.parent->scale;
	}

	length_[0] = size_.x * 0.5f * s.x;
	length_[1] = size_.y * 0.5f * s.y;
	length_[2] = size_.z * 0.5f * s.z;

	VECTOR3 t;
	t = VecNorm(VECTOR3(m._11, m._12, m._13));
	normaDirect_[0] = t;
	t = VecNorm(VECTOR3(m._21, m._22, m._23));
	normaDirect_[1] = t;
	t = VecNorm(VECTOR3(m._31, m._32, m._33));
	normaDirect_[2] = t;

	renderer_.Update(this);

	transform_.position = VECTOR3(m._41, m._42, m._43);
}
