//-----------------------------------------------------------------------------
//
//	2D描画[CanvasRenderer.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "CanvasRenderer.h"
#include "../../../Windows/Windows.h"
#include "../../GameSystems.h"

CanvasRenderer::CanvasRenderer(void) : texNum((int)Texture::Base::UNOWN), position(0), size(1)         , scale(1)      , scaleOffset(0), angle(0)
									 , rotationOffset(0)           , color(1)   , enable(true)    , type_(Wrapper::PRIMITIVE::TYPE::TRIANGLE_STRIP)
									 , pnum_(2)                    , priority_(0), buffer_(0)     , systems_(nullptr), pattern(0)    , split(1), shader(Shader::ENUM::UNKOUWN)
{
}

CanvasRenderer::~CanvasRenderer(void)
{
	if (systems_ != nullptr)
	{
		systems_->GetRenderer()->GetWrapper()->ReleaseBuffer(buffer_, Wrapper::FVF::VERTEX_2D);
		systems_->GetCanvasRenderer()->Remove(this);
	}
}

void CanvasRenderer::Init(Systems* systems, byte priority, int texNu)
{
	systems_ = systems;
	texNum = texNu;

	int vnum = 0;
	switch ((Texture::Base)texNum)
	{
	case Texture::Base::POINT:
		vnum = Wrapper::PRIMITIVE::V::POINT;
		break;
	case Texture::Base::LINE:
		vnum = Wrapper::PRIMITIVE::V::LINE;
		break;
	case Texture::Base::RECTANGLE:
		vnum = Wrapper::PRIMITIVE::V::RECT;
		Rectangle();
		break;
	case Texture::Base::FILL_TRIANGLE:
		vnum = Wrapper::PRIMITIVE::V::FILL_TRIANG;
		break;
	case Texture::Base::FILL_CIRCLE:
		vnum = Wrapper::PRIMITIVE::V::FILL_CIRCLE;
		break;
	default:
		vnum = Wrapper::PRIMITIVE::V::FILL_RECT;
		Sprite();
		break;
	}

//	uint n = systems->GetRenderer()->GetWrapper()->CreateVertexBuffer(vnum);
//	if (n == -1) { return; }

//	this->buffer = n;
	priority_ = priority;

	systems->GetCanvasRenderer()->Add(this);
}

void CanvasRenderer::Point(void)
{
	VERTEX2D v;

	v.position	= VECTOR4(0, 0, 1, 1);
	v.color		= COLOR(0, 0, 0, 0);
	v.texcoord	= VECTOR2(0, 0);

	type_ = Wrapper::PRIMITIVE::TYPE::POINT;
	pnum_ = 1;

	buffer_ = systems_->GetRenderer()->GetWrapper()->CreateVertexBuffer(&v, sizeof(VERTEX2D), 1);
}

void CanvasRenderer::Line(void)
{
	VERTEX2D v[2];

	for (int i = 0; i < 2; ++i)
	{
		v[i].position	= VECTOR4((float)(i % 2), 0, 1, 1);
		v[i].color		= COLOR(1, 1, 1, 1);
		v[i].texcoord	= VECTOR2(0, 0);
	}

	type_ = Wrapper::PRIMITIVE::TYPE::LINE;
	pnum_ = 2;

	buffer_ = systems_->GetRenderer()->GetWrapper()->CreateVertexBuffer(v, sizeof(VERTEX2D), 2);
}

void CanvasRenderer::Rectangle(void)
{
	VERTEX2D v[5];

	VECTOR2 p[4] =
	{
		{ 0, 0 },
		{ 1, 0 },
		{ 1, 1 },
		{ 0, 1 },
	};

	for (int i = 0; i < 4; ++i)
	{
		v[i].position	= VECTOR4(p[i].x, p[i].y, 1, 1);
		v[i].color		= COLOR(1, 1, 1, 1);
		v[i].texcoord	= VECTOR2(0, 0);
	}

	v[4] = v[0];

	type_ = Wrapper::PRIMITIVE::TYPE::LINELIST;
	pnum_ = 4;

	buffer_ = systems_->GetRenderer()->GetWrapper()->CreateVertexBuffer(v, sizeof(VERTEX2D), 5);
}

void CanvasRenderer::FillTriangle(void)
{
	float pc = (size.x * 0.5f - scaleOffset.x) * scale.x + (scaleOffset.x);
	float px = (-scaleOffset.x) * scale.x + (scaleOffset.x);
	float px_w = (size.x - scaleOffset.x) * scale.x + (scaleOffset.x);
	float py = (-scaleOffset.y) * scale.y + (scaleOffset.y);
	float py_h = (size.y - scaleOffset.y) * scale.y + (scaleOffset.y);

	//拡大縮小前に回転の中心をセットする
	pc -= rotationOffset.x;
	px -= rotationOffset.x;
	px_w -= rotationOffset.x;
	py -= rotationOffset.y;
	py_h -= rotationOffset.y;
	VECTOR2 pos = position;
	pos.x += rotationOffset.x - size.x * 0.5f;
	pos.y += rotationOffset.y - size.y * 0.5f;

	// FVF(今から使用する頂点情報)の設定
	VERTEX2D v[3];

	v[0].position = VECTOR4(pos.x + pc   * cosf(angle) - py   * sinf(angle),
							pos.y + pc   * sinf(angle) + py   * cosf(angle), 1.0f, 1.0f);
	v[1].position = VECTOR4(pos.x + px_w * cosf(angle) - py_h * sinf(angle),
							pos.y + px_w * sinf(angle) + py_h * cosf(angle), 1.0f, 1.0f);
	v[2].position = VECTOR4(pos.x + px   * cosf(angle) - py_h * sinf(angle),
							pos.y + px   * sinf(angle) + py_h * cosf(angle), 1.0f, 1.0f);

	v[0].color =
	v[1].color =
	v[2].color = color;

	v[0].texcoord = VECTOR2(0.0f, 0.0f);
	v[1].texcoord = VECTOR2(0.0f, 0.0f);
	v[2].texcoord = VECTOR2(0.0f, 0.0f);

//	systems->GetRenderer()->GetWrapper()->UpdateBuffer(buffer, &v[0], Wrapper::PRIMITIVE::V::FILL_TRIANG);

	type_ = Wrapper::PRIMITIVE::TYPE::TRIANGLE;
	pnum_ = 1;
}

void CanvasRenderer::FillRectangle(void)
{
	Sprite();
}

void CanvasRenderer::FillCircle(void)
{
	float theta = 0.0f;

	// FVF(今から使用する頂点情報)の設定
	VERTEX2D v[102];

	v[0].position = VECTOR4(position.x + -0.5f, position.y + -0.5f, 1.f, 1.f);
	v[0].color = color;
	v[0].texcoord = VECTOR2(0.0f, 0.0f);

	float r = size.x * 0.5f;

	for (int i = 1; i < Wrapper::PRIMITIVE::V::FILL_CIRCLE; i++)
	{
		v[i].position = VECTOR4(position.x + r * (float)cos(theta), position.y + r * (float)sin(theta), 1.0f, 1.0f);
		v[i].color = color;
		v[i].texcoord = VECTOR2(0.0f, 0.0f);
		theta += (float)(2 * 3.14f) / Wrapper::PRIMITIVE::V::CIRCUMFERENCE;
	}

//	systems->GetRenderer()->GetWrapper()->UpdateBuffer(buffer, &v[0], Wrapper::PRIMITIVE::V::FILL_CIRCLE);

	type_ = Wrapper::PRIMITIVE::TYPE::TRIANGLE_FAN;
	pnum_ = Wrapper::PRIMITIVE::V::CIRCUMFERENCE;
}

void CanvasRenderer::Animation(float add)
{
	pattern += add;	
	if (pattern >= split.x * split.y)
	{
		pattern = 0;
	}
}

void CanvasRenderer::Sprite(void)
{
	VERTEX2D v[4];

	for (int i = 0; i < 4; ++i)
	{
		v[i].position = VECTOR4((float)(i % 2), (float)(i / 2), 1, 1);
		v[i].color = COLOR(1, 1, 1, 1);
		v[i].texcoord = VECTOR2(0, 0);
	}

	type_ = Wrapper::PRIMITIVE::TYPE::TRIANGLE_STRIP;
	pnum_ = 2;

	buffer_ = systems_->GetRenderer()->GetWrapper()->CreateVertexBuffer(v, sizeof(VERTEX2D), 4);
}
