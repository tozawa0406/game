#include "Collider3DSphere.h"
#include "../../../Object/Object.h"

Collider3D::Sphere::Sphere(Object* obj) : Collider3DBase(obj, Type::SPHERE)
{
	renderer_.ColliderSphere(this, false);
	renderer2_.ColliderSphere(this, true);
}

void Collider3D::Sphere::Update(void)
{
	if (object_)
	{
		transform_ = object_->GetTransform();
		transform_.position = object_->GetTransform().globalPosition;
	}
	transform_.position += offsetPosition_;
	transform2_ = transform_;

	renderer_.enable = renderer2_.enable = IsEnable();

	if (systems_)
	{
		if (const auto& debug = systems_->GetDebug())
		{
			if (!debug->GetDebug()) { return; }
		}
	}

	renderer_.Update(this);
	renderer2_.Update(this);
}
