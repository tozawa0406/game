#include "Collider3DOBB.h"
#include "../../../Object/Object.h"

#include "../../../../../Sources/Object/GameObject/GameObject.h"

Collider3D::OBB::OBB(Object* obj) : Collider3DBase(obj, Type::OBB)
	, velocity_(VECTOR3(0))
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
	if (object_)
	{
		if (object_->GetTag() == Object::Tag::PLAYER ||
			object_->GetTag() == Object::Tag::ENEMY)
		{
			auto gameObject = static_cast<GameObject*>(object_);
			velocity_ = gameObject->GetVelocity();
		}
		transform_ = object_->GetTransform();
	}
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
