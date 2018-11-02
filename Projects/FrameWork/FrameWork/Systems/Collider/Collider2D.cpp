//-----------------------------------------------------------------------------
//
//	2D“–‚½‚è”»’è[Collider2D.cpp]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Collider2D.h"
#include <math.h>
#include "../../Object/Object.h"
#include "../../Scene/SceneManager.h"
#include "../Camera/CameraManager.h"
#include "../GameSystems.h"

Collider2DBase::Collider2DBase(Object* obj, Type type) : systems_(Systems::Instance()), transform_(&obj->GetTransform()), object_(obj), type_(type),  enable_(false)
{
	position_	= VECTOR3(0);
	offset_		= VECTOR3(0);
	size_		= VECTOR2(0);

	systems_->GetCollision2D()->Add(this);
}

Collider2DBase::~Collider2DBase(void)
{
	systems_->GetCollision2D()->Remove(this);
}

Collider2D::Circle::Circle(Object* obj) : Collider2DBase(obj, Type::CIRCLE)
{
	renderer_.ColliderCircle(this);
}

void Collider2D::Circle::Update(void)
{
	position_	= transform_->position + offset_;

	if (!systems_->GetDebug()->GetDebug()) { return; }

	renderer_.Update(this);
}

Collider2D::Rectangle::Rectangle(Object* obj) : Collider2DBase(obj, Type::RECTANGLE)
{
	renderer_.ColliderRectangle(this);
}

void Collider2D::Rectangle::Update(void)
{
	position_	= transform_->position + offset_;

	if (!systems_->GetDebug()->GetDebug()) { return; }

	renderer_.Update(this);
}

std::vector<Object*> Collider2DBase::Hit(void)
{
	return list_;
}

void Collider2DBase::HitStop(Object* hitObj, VECTOR3& pos, VECTOR2& velocity)
{
	VECTOR2 dir = V2(hitObj->GetTransform().position - pos);
	float dot = VecDot(VecNorm(velocity), VecNorm(dir));
	if (dot > 0)
	{
		pos -= V3(velocity);
		velocity = 0;
	}
}
