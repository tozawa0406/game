//-----------------------------------------------------------------------------
//
//	Auther : ���V�đ�
//-----------------------------------------------------------------------------
#include "Dragon.h"

//! @def	�傫��
static constexpr float SCALE = 1.75f;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
Dragon::Dragon(void) : Object(Object::Tag::ENEMY)
{
	for (auto& c : collision_)
	{
		c = nullptr;
	}
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
Dragon::~Dragon(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
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
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Dragon::Uninit(void)
{
	for (auto& c : collision_)
	{
		DeletePtr(c);
	}
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
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
