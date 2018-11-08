//-----------------------------------------------------------------------------
//
//	当たり判定描画[ColliderRenderer.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "ColliderRenderer.h"
#include <math.h>
#include "../GameSystems.h"
#include "../Renderer/Sprite/SpriteRenderer.h"
#include "Collider2D.h"
#include "Collider3D.h"

//ポリゴン描画処理
void ColliderRendererManager::Draw(void)
{
#ifdef _SELF_DEBUG
	if (!systems_->GetDebug()->GetDebug()) { return; }

	const auto& dev = systems_->GetRenderer()->GetWrapper();

	dev->BeginDrawObjectRenderer();
	for (auto& obj : obj_)
	{
		if (obj->enable)
		{
			dev->Draw(obj);
		}
	}
	dev->EndDrawObjectRenderer();
#endif
}



ColliderRenderer::ColliderRenderer(void) : enable(false), vertexBuffer(0), vnum(0)
										 , type(Wrapper::PRIMITIVE::TYPE::POINT), offsetPosition(VECTOR3(0)), offsetRotation(VECTOR3(0)), size(VECTOR3(0))
										 , parentMtx(nullptr), transMtx(nullptr), color(COLOR::RGBA(35, 191, 0, 255))
{
	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(0, 0, 0);
	transform.scale    = VECTOR3(1, 1, 1);
}

ColliderRenderer::~ColliderRenderer(void)
{
#ifdef _SELF_DEBUG
	const auto& dev = systems->GetRenderer()->GetWrapper();
	dev->ReleaseBuffer(vertexBuffer, Wrapper::FVF::VERTEX_3D);

	systems->GetColliderRendererManager()->Remove(this);
#endif
}

void ColliderRenderer::Update(const Collider3DBase* col)
{
	transform = col->transform_;
	size      = col->size_;
	enable    = col->enable_;
	parentMtx = col->parentMtx_;
	transMtx  = col->transMtx_;

	offsetPosition = col->offsetPosition_;
	offsetRotation = col->offsetRotation_;
}

void ColliderRenderer::Update(const Collider2DBase* col)
{
	this->transform.position = col->transform_->position;
	this->transform.rotation = col->transform_->rotation;
	this->enable = col->enable_;
}

void ColliderRenderer::ColliderRectangle(const Collider2DBase* col)
{
#ifdef _SELF_DEBUG
	const int vn = 5;

	VECTOR3 p[vn - 1] =
	{
		{ -0.5f, -0.5f, 0 },
		{  0.5f, -0.5f, 0 },
		{  0.5f,  0.5f, 0 },
		{ -0.5f,  0.5f, 0 }
	};


	VERTEX3D v[vn];

	for (int i = 0; i < vn; ++i)
	{
		v[i].position = p[i];
		v[i].normal   = VECTOR3(0, 0, 0);
		v[i].color    = COLOR::RGBA(35, 191, 0, 255);
		v[i].texcoord = VECTOR2(0, 0);
	}

	v[4] = v[0];

	vnum = vn;
	type = Wrapper::PRIMITIVE::TYPE::LINELIST;
	pnum = 4;

	this->systems = col->systems_;
	const auto& dev = systems->GetRenderer()->GetWrapper();
	vertexBuffer = dev->CreateVertexBuffer(v, sizeof(v[0]), vnum);
	this->vnum = vnum;
	systems->GetColliderRendererManager()->Add(this);
#else
	UNREFERENCED_PARAMETER(col);
#endif
}

void ColliderRenderer::ColliderCircle(const Collider2DBase* col)
{
#ifdef _SELF_DEBUG
	float theta = 0.0f;

	float r = col->size_.x * 0.5f;

	VECTOR3 p[Wrapper::PRIMITIVE::V::CIRCUMFERENCE + 1];
	for (int i = 0; i < Wrapper::PRIMITIVE::V::CIRCUMFERENCE + 1; ++i)
	{
		p[i] = { r * (float)cos(theta), r * (float)sin(theta), 0 };
		theta += (float)(2 * 3.14f) / Wrapper::PRIMITIVE::V::CIRCUMFERENCE;
	}

	// FVF(今から使用する頂点情報)の設定
	VERTEX3D v[Wrapper::PRIMITIVE::V::CIRCUMFERENCE + 1];

	for (int i = 0; i < Wrapper::PRIMITIVE::V::CIRCUMFERENCE + 1; ++i)
	{
		v[i].position = p[i];
		v[i].normal   = VECTOR3(0, 0, 0);
		v[i].color    = COLOR::RGBA(35, 191, 0, 255);
		v[i].texcoord = VECTOR2(0.0f, 0.0f);
	}

	vnum = Wrapper::PRIMITIVE::V::CIRCUMFERENCE + 1;
	type = Wrapper::PRIMITIVE::TYPE::LINELIST;
	pnum = Wrapper::PRIMITIVE::V::CIRCUMFERENCE;

	this->systems = col->systems_;
	const auto& dev = systems->GetRenderer()->GetWrapper();
	vertexBuffer = dev->CreateVertexBuffer(v, sizeof(v[0]), vnum);
	this->vnum = vnum;
	systems->GetColliderRendererManager()->Add(this);
#else
	UNREFERENCED_PARAMETER(col);
#endif
}

void ColliderRenderer::ColliderSegment(const Collider3DBase* col)
{
	this->systems = col->systems_;
#ifdef _SELF_DEBUG
	VECTOR3 p[2] =
	{
		{              - 0.5f,              - 0.5f,              - 0.5f},
		{ col->size_.x - 0.5f, col->size_.y - 0.5f, col->size_.z - 0.5f}
	};

	// FVF(今から使用する頂点情報)の設定
	VERTEX3D v[Wrapper::PRIMITIVE::V::COLLIDER_SEGMENT];

	for (int i = 0; i < Wrapper::PRIMITIVE::V::COLLIDER_SEGMENT; ++i)
	{
		v[i].position = p[i];
		v[i].normal   = VECTOR3(0, 0, 0);
		v[i].color    = COLOR::RGBA(35, 191, 0, 255);
		v[i].texcoord = VECTOR2(0.0f, 0.0f);
	}

	type = Wrapper::PRIMITIVE::TYPE::LINELIST;
	pnum = 1;

	const auto& dev = systems->GetRenderer()->GetWrapper();
	vertexBuffer = dev->CreateVertexBuffer(v, sizeof(v[0]), vnum);
	this->vnum = vnum;
	systems->GetColliderRendererManager()->Add(this);
#else
	UNREFERENCED_PARAMETER(col);
#endif
}

void ColliderRenderer::ColliderSphere(const Collider3DBase* col, bool second)
{
	this->systems = col->systems_;
#ifdef _SELF_DEBUG
	float theta = 0.0f;

	float r = col->size_.x * 0.5f;

	VECTOR3 p[Wrapper::PRIMITIVE::V::COLLIDER_SPHERE];
	for (int i = 0; i < Wrapper::PRIMITIVE::V::CIRCUMFERENCE; ++i)
	{
		if (!second)
		{
			p[i] = { r * (float)cos(theta), r * (float)sin(theta), 0 };
		}
		else
		{
			p[i] = { 0, r * (float)sin(theta), r * (float)cos(theta) };
		}
		theta += (float)(2 * 3.14f) / Wrapper::PRIMITIVE::V::CIRCUMFERENCE;
	}

	// FVF(今から使用する頂点情報)の設定
	VERTEX3D v[Wrapper::PRIMITIVE::V::COLLIDER_SPHERE];

	for (int i = 0; i < Wrapper::PRIMITIVE::V::CIRCUMFERENCE; ++i)
	{
		v[i].position = p[i];
		v[i].normal   = VECTOR3(0, 0, 0);
		v[i].color    = COLOR::RGBA(35, 191, 0, 255);
		v[i].texcoord = VECTOR2(0, 0);
	}
	v[Wrapper::PRIMITIVE::V::CIRCUMFERENCE] = v[0];

	vnum = Wrapper::PRIMITIVE::V::COLLIDER_SPHERE;
	type = Wrapper::PRIMITIVE::TYPE::LINELIST;
	pnum = Wrapper::PRIMITIVE::V::CIRCUMFERENCE;

	const auto& dev = systems->GetRenderer()->GetWrapper();
	vertexBuffer = dev->CreateVertexBuffer(v, sizeof(v[0]), vnum);
	this->vnum = vnum;
	systems->GetColliderRendererManager()->Add(this);
#else
	UNREFERENCED_PARAMETER(second);
#endif
}

void ColliderRenderer::ColliderOBB(const Collider3DBase* col)
{
	const int vn = 16;

	VECTOR3 p[vn] =
	{
		{ -0.5f, -0.5f, -0.5f }, 
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f,  0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f, -0.5f,  0.5f },
	};

	VERTEX3D v[vn];

	for (int i = 0; i < vn; ++i)
	{
		v[i].position = p[i];
		v[i].normal   = VECTOR3(0, 0, 0);
		v[i].color    = COLOR(1, 1, 1, 1);
		v[i].texcoord = VECTOR2(0, 0);
	}

	vnum = vn;
	type = Wrapper::PRIMITIVE::TYPE::LINELIST;
	pnum = vn - 1;

	this->systems = col->systems_;
	const auto& dev = systems->GetRenderer()->GetWrapper();
	vertexBuffer = dev->CreateVertexBuffer(v, sizeof(v[0]), vnum);
	systems->GetColliderRendererManager()->Add(this);
}
