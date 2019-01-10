#include "WallParts.h"

WallParts::WallParts(void) : Object(ObjectTag::STATIC)
	, collider_(nullptr)
{
}

WallParts::~WallParts(void)
{
}

void WallParts::Init(void)
{	
	collider_ = new Collider3D::OBB(this);
}

void WallParts::Uninit(void)
{
	DeletePtr(collider_);
}

void WallParts::SetColliderOffset(const VECTOR3& position, const VECTOR3& rotation)
{
	if (collider_)
	{
		collider_->SetOffsetPosition(position);
		collider_->SetOffsetRotation(rotation);
	}
}

