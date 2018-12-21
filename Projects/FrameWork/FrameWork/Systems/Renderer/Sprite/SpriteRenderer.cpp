//-----------------------------------------------------------------------------
//
//	板ポリ描画[SpriteRenderer.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "SpriteRenderer.h"
#include "../../../Windows/Windows.h"
#include "../../GameSystems.h"

#include "../Shader/Default.h"

SpriteRenderer::SpriteRenderer(void) : ObjectRenderer(ObjectRenderer::RendererType::SPRITE), texNum((int)Texture::Base::UNOWN)
									 , vertexBuffer(0), indexBuffer(0), vertexNum(4), indexNum(6), pattern(0), split(1), billbord(false), xBillbord(false)
{
	texcoord = VECTOR4(0, 0, 1, 1);
}

SpriteRenderer::~SpriteRenderer(void)
{
	if (systems_)
	{
		systems_->GetGraphics()->GetWrapper()->ReleaseBuffer(vertexBuffer, Wrapper::FVF::VERTEX_3D);
	}
}

void SpriteRenderer::Init(Systems* systems, int texNu, const Transform* transform)
{
	ObjectRenderer::Init(systems, transform);

	this->texNum = texNu;

	VECTOR2 inv = { 1 / split.x, 1 / split.y };
	int		splitX = (int)split.x;
	int		p = (int)pattern;

	// 左上座標
	texcoord.x = inv.x *  (p % splitX);
	texcoord.y = inv.y *  (p / splitX);

	// サイズ
	texcoord.z = inv.x;
	texcoord.w = inv.y;

	const auto& wrapper = systems->GetGraphics()->GetWrapper();

	VERTEX v[4];
	for (int i = 0; i < 4; i++)
	{
		int x = i % 2, y = i / 2;
		v[i].position	= VECTOR3(-0.5f + (1 * x), 0.5f - (1 * y), 0.0f);
		v[i].normal		= VECTOR3(0, 0, -1);
		v[i].tangent	= VECTOR3(0, 0, 0);
		v[i].color		= COLOR(1, 1, 1, 1);
		v[i].texcoord	= VECTOR2((float)x, (float)y);
		v[i].boneIndex	= VECTOR4(0, 0, 0, 0);
		v[i].weight		= VECTOR4(0, 0, 0, 0);
	}
	this->vertexBuffer = wrapper->CreateVertexBuffer(v, sizeof(v[0]), Wrapper::PRIMITIVE::V::FILL_RECT);
	if (this->vertexBuffer == Wrapper::R_ERROR) { return; }

	WORD index[6];
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 2;
	index[4] = 3;
	index[5] = 1;
	this->indexBuffer = wrapper->CreateIndexBuffer(index, 6);

	shader = Shader::ENUM::DEFAULT;
}

bool SpriteRenderer::Animation(float add)
{
	pattern += add;

	bool zero = false;
	if (pattern >= split.x * split.y)
	{
		pattern = 0;
		zero = true;
	}

	VECTOR2 inv		= { 1 / split.x, 1 / split.y };
	int		splitX	= (int)split.x;
	int		p		= (int)pattern;

	// 左上座標
	texcoord.x = inv.x *  (p % splitX);
	texcoord.y = inv.y *  (p / splitX);

	// サイズ
	texcoord.z = inv.x;
	texcoord.w = inv.y;

	return zero;
}
