#include "Collider3DSegment.h"
#include "../../../Object/Object.h"

Collider3D::Segment::Segment(Object* obj) : Collider3DBase(obj, Type::SEGMENT)
{
	renderer_.ColliderSegment(this);
}

void Collider3D::Segment::Update(void)
{
	transform_ = object_->GetTransform();
	transform_.position += offsetPosition_;

	renderer_.enable = false;
	if (systems_)
	{
		if (const auto& debug = systems_->GetDebug())
		{
			if (!debug->GetDebug()) { return; }
		}
	}

	renderer_.Update(this);
}
