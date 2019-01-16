#include "Cannon.h"
#include <FrameWork/Object/ObjectManager.h>
#include "StaticObject.h"

Cannon::Cannon(void) : Object(ObjectTag::STATIC)
{
}

Cannon::~Cannon(void)
{
}

void Cannon::Init(void)
{
	if (auto cannon = manager_->Create<StaticObject>(static_cast<uint8>(Model::Game::CANNON)))
	{
		cannon->SetTransform(Transform(VECTOR3(-60, 1.5f, 0), VECTOR3(0, -1.7f, 0), VECTOR3(2)));
		cannon->SetColliderOffset(VECTOR3(0, 5, 3), VECTOR3(0), VECTOR3(12.5f, 10, 31));
	}
	if (auto barrel = manager_->Create<StaticObject>(static_cast<uint8>(Model::Game::CANNON_BARREL)))
	{
		barrel->SetTransform(Transform(VECTOR3(-65, 2.5f, 20), VECTOR3(-0.03f, -1.3f,0), VECTOR3(2)));
		barrel->SetColliderOffset(VECTOR3(0, 0, 10.5f), VECTOR3(0), VECTOR3(10, 10, 32.5f));
	}


	if (auto bullet = manager_->Create<StaticObject>(static_cast<uint8>(Model::Game::CANNON_BULLET)))
	{
		bullet->SetTransform(Transform(VECTOR3(-45, 0.9f, 10), VECTOR3(0), VECTOR3(3)));
		bullet->SetColliderOffset(VECTOR3(0), VECTOR3(0), VECTOR3(4));
	}
	if (auto bullet = manager_->Create<StaticObject>(static_cast<uint8>(Model::Game::CANNON_BULLET)))
	{
		bullet->SetTransform(Transform(VECTOR3(-45, 0.9f, 13), VECTOR3(0), VECTOR3(3)));
		bullet->SetColliderOffset(VECTOR3(0), VECTOR3(0), VECTOR3(4));
	}
	if (auto bullet = manager_->Create<StaticObject>(static_cast<uint8>(Model::Game::CANNON_BULLET)))
	{
		bullet->SetTransform(Transform(VECTOR3(-42, 0.9f, 11.5f), VECTOR3(0), VECTOR3(3)));
		bullet->SetColliderOffset(VECTOR3(0), VECTOR3(0), VECTOR3(4));
	}
	if (auto bullet = manager_->Create<StaticObject>(static_cast<uint8>(Model::Game::CANNON_BULLET)))
	{
		bullet->SetTransform(Transform(VECTOR3(-43.5f, 2.9f, 11.5f), VECTOR3(0), VECTOR3(3)));
		bullet->SetColliderOffset(VECTOR3(0), VECTOR3(0), VECTOR3(4));
	}

}

void Cannon::Uninit(void)
{
}
