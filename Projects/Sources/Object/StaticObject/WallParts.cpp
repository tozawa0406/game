#include "WallParts.h"

WallParts::WallParts(Model::Game model) : Object(ObjectTag::STATIC)
	, collider_(nullptr)
	, model_(model)
{
}

WallParts::~WallParts(void)
{
}

void WallParts::Init(void)
{	
	mesh_.Init(Systems::Instance(), static_cast<int>(model_), &transform_);

	collider_ = new Collider3D::OBB(this);
	if (!collider_) { return; }

	// 手前左
	if (model_ == Model::Game::ROCK_1)
	{
		transform_ = Transform(VECTOR3(-80, 0, -80), VECTOR3(0), VECTOR3(2));
		collider_->SetSize(VECTOR3(30));
	}
	// 手前正面
	else if (model_ == Model::Game::ROCK_14)
	{
		transform_ = Transform(VECTOR3(0, -5, -90), VECTOR3(0.3f, 0, 0), VECTOR3(3));
		collider_->SetOffsetPosition(VECTOR3(-8.5f, 15, 0));
		collider_->SetOffsetRotation(VECTOR3(-0.9f, 0, 0));
		collider_->SetSize(VECTOR3(45, 30, 25));
	}
	// 手前右
	else if(model_ == Model::Game::ROCK_12)
	{
		transform_ = Transform(VECTOR3(80, 0, -80), VECTOR3(0, 1.57f, 0), VECTOR3(3));
		collider_->SetSize(VECTOR3(20, 60, 30));
	}
	// 手前右真ん中
	else if (model_ == Model::Game::ROCK_4)
	{
		transform_ = Transform(VECTOR3(80, 0, -60), VECTOR3(0, 3.14f, 0), VECTOR3(2.5f));
		collider_->SetSize(VECTOR3(20));
	}
	// 手前右奥
	else if (model_ == Model::Game::ROCK_10)
	{
		transform_ = Transform(VECTOR3(80, 0, -40), VECTOR3(0, 3.14f, 0), VECTOR3(2.5f));
		collider_->SetOffsetPosition(VECTOR3(0, 0, 5));
		collider_->SetSize(VECTOR3(15, 20, 15));
	}
}

void WallParts::Uninit(void)
{
	DeletePtr(collider_);
}
