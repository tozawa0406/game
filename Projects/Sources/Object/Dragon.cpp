//-----------------------------------------------------------------------------
//
//	Auther : 戸澤翔太
//-----------------------------------------------------------------------------
#include "Dragon.h"
#include <FrameWork/Systems/Input/Controller.h>

//! @def	大きさ
static constexpr float SCALE = 0.9f;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
Dragon::Dragon(void) : Object(Object::Tag::ENEMY), animNum_(0)
{
	for (auto& c : collision_)
	{
		c = nullptr;
	}
}

/* @fn		デストラクタ
 * @brief	...						*/
Dragon::~Dragon(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	なし
 * @return	なし					*/
void Dragon::Init(void)
{
	transform_.position		= VECTOR3(0, 0, 0);
	transform_.rotation.y   = PI;
	transform_.scale		= VECTOR3(SCALE);

	mesh_.Init(Systems::Instance(), (int)Model::Game::DRAGON, &transform_);

	auto  s = transform_.scale;
	auto& c1 = collision_[static_cast<int>(Collision::BODY)];
	c1 = new Collider3D::OBB(this);
	if (c1)
	{
		c1->SetOffset(VECTOR3(0, 8, 5) * s);
		c1->SetSize(VECTOR3(5, 8, 10) * s);
		c1->Update();
	}

	auto& c2 = collision_[static_cast<int>(Collision::HEAD)];
	c2 = new Collider3D::OBB(this);
	if (c2)
	{
		c2->SetOffset(VECTOR3(0, 14, 11) * s);
		c2->SetSize(VECTOR3(3, 3, 4) * s);
		c2->Update();
	}

	auto& c3 = collision_[static_cast<int>(Collision::TAIL)];
	c3 = new Collider3D::OBB(this);
	if (c3)
	{
		c3->SetOffset(VECTOR3(0, 5, -15) * s);
		c3->SetSize(VECTOR3(2, 2, 20) * s);
		c3->Update();
	}

	auto& c4 = collision_[static_cast<int>(Collision::WING_R)];
	c4 = new Collider3D::OBB(this);
	if (c4)
	{
		c4->SetOffset(VECTOR3(10, 14, 0) * s);
		c4->SetSize(VECTOR3(5, 5, 5) * s);
		c4->Update();
	}

	auto& c5 = collision_[static_cast<int>(Collision::WING_L)];
	c5 = new Collider3D::OBB(this);
	if (c5)
	{
		c5->SetOffset(VECTOR3(-10, 14, 0) * s);
		c5->SetSize(VECTOR3(5, 5, 5) * s);
		c5->Update();
	}
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void Dragon::Uninit(void)
{
	for (auto& c : collision_)
	{
		DeletePtr(c);
	}
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	なし					*/
void Dragon::Update(void)
{
	COLOR color = COLOR(1, 1, 1, 1);
	for (auto& c : collision_)
	{
		if (c)
		{
			c->Update();

			auto hits = c->Hit();
			for (auto& hit : hits)
			{
				const auto& tag = hit->GetTag();
				if (tag == Object::Tag::WAPON)
				{
					color = COLOR(1, 0, 0, 1);
				}
				if (tag == Object::Tag::PLAYER)
				{
					color = COLOR(0, 0, 0, 1);
				}
			}
		}
	}
	mesh_.material.diffuse = color;


	mesh_.Animation(0.75f);

	if (GetCtrl(0)->Trigger(Input::GAMEPAD_L3, DIK_LCONTROL))
	{
		animNum_++;
		if (animNum_ >= static_cast<uint8>(Animation::MAX)) { animNum_ = 0; }
		mesh_.ChangeAnimation(animNum_, 30);
	}
}
