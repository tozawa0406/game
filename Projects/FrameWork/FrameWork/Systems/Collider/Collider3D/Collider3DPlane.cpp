#include "Collider3DPlane.h"
#include "../../../Object/Object.h"

Collider3D::Plane::Plane(Object* obj) : Collider3DBase(obj, Type::PLANE)
	, normal_(VECTOR3(0))
{
	transform_ = object_->GetTransform();
}

void Collider3D::Plane::Update(void)
{
	transform_ = object_->GetTransform();
	transform_.position += offsetPosition_;
	transform_.rotation += offsetRotation_;
}

void Collider3D::Plane::SetNormal(const VECTOR3& normal)
{
	MATRIX rot = MATRIX().Identity().Rotation(transform_.rotation);
	normal_ = VecTransform(normal, rot);
}
