//-----------------------------------------------------------------------------
//
//	Auther : 戸澤翔太
//-----------------------------------------------------------------------------
#include "Dragon.h"

//! @def	大きさ
static constexpr float SCALE = 1.75f;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
Dragon::Dragon(void) : Object(Object::Tag::ENEMY)
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
	transform_.position		= VECTOR3(0, 0, 50);
	transform_.rotation.y	= PI;
	transform_.scale		= VECTOR3(SCALE);

	mesh_.Init(Systems::Instance(), (int)Model::Game::DRAGON, &transform_);

	auto  s = transform_.scale;
	auto& c = collision_[static_cast<int>(Collision::BODY)];
	c = new Collider3D::OBB(this);
	if (c)
	{
		c->SetOffset(VECTOR3(0, 8, -3) * s);
		c->SetSize(VECTOR3(5, 8, 10) * s);
		c->Update();
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
		}

		auto hits = c->Hit();
		for (auto& hit : hits)
		{
			if (hit->GetTag() == Object::Tag::WAPON)
			{
				color = COLOR(1, 0, 0, 1);
			}
		}
	}
	mesh_.material.diffuse = color;
}
