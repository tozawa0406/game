#include "CampWall.h"
#include <FrameWork/Object/ObjectManager.h>
#include "CampWallParts.h"
#include "StaticObject.h"

CampWall::CampWall(void) : Object(ObjectTag::STATIC)
{
}

CampWall::~CampWall(void)
{
}

void CampWall::Init(void)
{
	// 壁
	manager_->Create<CampWallParts>(VECTOR3( 100, 3,    0), VECTOR3(0,  1.57f, 0));
	manager_->Create<CampWallParts>(VECTOR3(   0, 3, -100), VECTOR3(0,     0 , 0));
	manager_->Create<CampWallParts>(VECTOR3(-100, 3,    0), VECTOR3(0, -1.57f, 0));
	manager_->Create<CampWallParts>(VECTOR3(   0, 3,  100), VECTOR3(0,  3.14f, 0));

	// 手前左
	if (auto stone = manager_->Create<StaticObject>(static_cast<uint8>(Resources::Model::Camp::ROCK_1)))
	{
		stone->SetTransform(Transform(VECTOR3(-80, 0, -80), VECTOR3(0), VECTOR3(2)));
		stone->SetColliderOffset(VECTOR3(0), VECTOR3(0), VECTOR3(30));
	}
	// 手前正面
	if (auto stone = manager_->Create<StaticObject>(static_cast<uint8>(Resources::Model::Camp::ROCK_14)))
	{
		stone->SetTransform(Transform(VECTOR3(0, -5, -90), VECTOR3(0.3f, 0, 0), VECTOR3(3)));
		stone->SetColliderOffset(VECTOR3(-8.5f, 15, 0), VECTOR3(-0.9f, 0, 0), VECTOR3(45, 30, 25));
	}
	// 手前右
	if (auto stone = manager_->Create<StaticObject>(static_cast<uint8>(Resources::Model::Camp::ROCK_12)))
	{
		stone->SetTransform(Transform(VECTOR3(80, 0, -80), VECTOR3(0, 1.57f, 0), VECTOR3(3)));
		stone->SetColliderOffset(VECTOR3(0), VECTOR3(0), VECTOR3(20, 60, 30));
	}
	// 手前右真ん中
	if (auto stone = manager_->Create<StaticObject>(static_cast<uint8>(Resources::Model::Camp::ROCK_4)))
	{
		stone->SetTransform(Transform(VECTOR3(80, 0, -60), VECTOR3(0, 3.14f, 0), VECTOR3(2.5f)));
		stone->SetColliderOffset(VECTOR3(0), VECTOR3(0), VECTOR3(20));
	}
	// 手前右奥
	if (auto stone = manager_->Create<StaticObject>(static_cast<uint8>(Resources::Model::Camp::ROCK_4)))
	{
		stone->SetTransform(Transform(VECTOR3(80, 0, -40), VECTOR3(0, 3.14f, 0), VECTOR3(2.5f)));
		stone->SetColliderOffset(VECTOR3(0), VECTOR3(0), VECTOR3(15, 20, 20));
	}
}

void CampWall::Uninit(void)
{
}
