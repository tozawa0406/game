#include "Collider3DBase.h"
#include "Collider3DManager.h"
#include "../../GameSystems.h"
#include "../../../Object/Object.h"
#include "../../../Object/ObjectManager.h"

#include "../../../../../Sources/Object/GameObject/GameObject.h"

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
Collider3DBase::Collider3DBase(Object* obj, Type type)
	: systems_(Systems::Instance())
	, transform_(Transform(VECTOR3(0), VECTOR3(0), VECTOR3(1)))
	, object_(obj)
	, type_(type)
	, enable_(true)
	, parentMtx_(nullptr)
	, transMtx_(nullptr)
	, offsetPosition_(VECTOR3(0))
	, offsetRotation_(VECTOR3(0))
	, size_(VECTOR3(1))
{
	if (systems_)
	{
		if (const auto& manager = systems_->GetCollision3D())
		{
			manager->Add(this);
		}
	}
}

/* @fn		デストラクタ
 * @brief	メモリの解放			*/
Collider3DBase::~Collider3DBase(void)
{
	if (systems_)
	{
		if (const auto& manager = systems_->GetCollision3D())
		{
			manager->Remove(this);
		}
	}
}

void Collider3DBase::Init(void)
{
	if (object_)
	{
		transform_ = object_->GetTransform();
	}
}
