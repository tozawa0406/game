#include "StaticObject.h"

StaticObject::StaticObject(uint8 model) : Object(ObjectTag::STATIC)
	, collider_(nullptr)
	, model_(model)
{
}

StaticObject::~StaticObject(void)
{
}

void StaticObject::Init(void)
{	
	mesh_.Init(Systems::Instance(), static_cast<int>(model_), &transform_);

	collider_ = new Collider3D::OBB(this);
}

void StaticObject::Uninit(void)
{
	DeletePtr(collider_);
}

void StaticObject::SetColliderOffset(VECTOR3 position, VECTOR3 rotation, VECTOR3 size)
{
	if (collider_)
	{
		collider_->SetOffsetPosition(position);
		collider_->SetOffsetRotation(rotation);
		collider_->SetSize(size);
	}
}
