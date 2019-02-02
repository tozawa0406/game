#include "GameObject.h"
#include <FrameWork/Object/ObjectManager.h>
#include <FrameWork/Systems/Renderer/MeshField.h>


GameObject::GameObject(ObjectTag tag) : Object(tag)
	, life_(0)
	, ded_(false)
	, isEndAnim_(false)
	, velocity_(VECTOR3(0))
	, front_(VECTOR3(0))
	, right_(VECTOR3(0))
{
}

GameObject::~GameObject(void)
{
}

void GameObject::Move(void)
{
	// キャラクターの前ベクトルの生成
	CreateFrontVector();

	// 移動向きによりキャラクターの向きを変える
	if ((Abs(velocity_.x) + Abs(velocity_.z) > 0.02f))
	{
		VECTOR3 velocityNorm = VecNorm(velocity_);
		VECTOR3 frontVelocityCross = VecCross(front_, velocityNorm);
		float	dot = VecDot(front_, velocityNorm);

		// 前か後ろに進みたいとき
		int sign = 1;
		if (frontVelocityCross.y < 0) { sign = -1; }

		int upY = (int)(((frontVelocityCross.y * 10) + (5 * sign)) * 0.1f);
		// 内積が0以下の時(後ろに進むとき)
		if (upY == 0 && dot == 1)
		{
			// 強制的に回す
			frontVelocityCross.y = 1.0f * sign;
		}

		transform_.rotation.y += frontVelocityCross.y * 0.3f;
	}
	else if (Abs(velocity_.y) > 0.02f) {}
	else { velocity_ = VECTOR3(0); }

	OnGround();

	transform_.position += velocity_;		// 移動

	velocity_ *= 0.8f;						// 慣性
}

void GameObject::CreateFrontVector(void)
{
	MATRIX frontObj;
	frontObj.Identity().Translation(VECTOR3(0, 0, 1));
	MATRIX mtx;
	mtx.Identity().Rotation(VECTOR3(0, transform_.rotation.y, 0));
	mtx.Translation(transform_.position);
	frontObj *= mtx;

	VECTOR3 tempTarget = VECTOR3(frontObj._41, transform_.position.y, frontObj._43);
	front_ = transform_.position - tempTarget;
	front_ = VecNorm(front_);

	right_ = VecNorm(VecCross(VECTOR3(0, 1, 0), front_));
}

void GameObject::OnGround(void)
{
	if (manager_)
	{
		if (const auto& scene = manager_->GetScene())
		{
			if (const auto& meshfield = scene->GetMeshField())
			{
				transform_.position = meshfield->Hit(transform_.position, velocity_);
			}
		}
	}
}

void GameObject::CreateDefenseCollider(const Collider3D::OBB& normal, Collider3D::OBB* copy)
{
	if (!copy) { return; }

	if (copy)
	{
		for (int i = 0; i < 3; ++i)
		{
			copy->SetDirect(i, normal.GetDirect(i));
		}
		copy->SetSize(normal.GetSize() - VECTOR3(0.1f));
		copy->SetOffsetPosition(normal.GetOffsetPosition());
		copy->SetOffsetRotation(normal.GetOffsetRotation());
		copy->SetParentMtx(normal.GetTransMtx(), normal.GetParentMtx());
		copy->SetColliderTag(ColliderTag::DEFENSE);
		copy->SetTrigger(true);
		copy->SetEnable(normal.IsEnable());
	}
}
