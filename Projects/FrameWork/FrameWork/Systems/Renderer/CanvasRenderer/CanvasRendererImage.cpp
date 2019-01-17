#include "CanvasRendererImage.h"

CanvasRenderer::Image::Image(void) : CanvasRendererBase(CanvasType::Image)
	, texNum_(static_cast<int>(Resources::Texture::Base::UNOWN))
	, scale_(VECTOR2(1))
	, scaleOffset_(VECTOR2(0))
	, angle_(0)
	, rotationOffset_(VECTOR2(0))
	, shader_(Shader::ENUM::UNKOUWN)
	, pattern_(0)
	, split_(VECTOR2(1))
	, primitiveType_(Wrapper::PRIMITIVE::TYPE::TRIANGLE_STRIP)
	, primitiveNum_(2)
	, buffer_(0)
{
}

CanvasRenderer::Image::~Image(void)
{
}

void CanvasRenderer::Image::Init(byte priority, int texNum)
{
	priority_	= priority;
	OnInit();
	texNum_ = texNum;

	CreateVertexBuffer();
}

void CanvasRenderer::Image::Uninit(void)
{
	if (manager_)
	{
		if (const auto& systems = manager_->GetSystems())
		{
			if (const auto& graphics = systems->GetGraphics())
			{
				if (const auto& wrapper = graphics->GetWrapper())
				{
					wrapper->ReleaseBuffer(buffer_, Wrapper::FVF::VERTEX_2D);
				}
			}
		}
	}

	OnUninit();
}

void CanvasRenderer::Image::CreateVertexBuffer(void)
{
	VERTEX2D v[4];

	for (int i = 0; i < 4; ++i)
	{
		v[i].position	= VECTOR4((i % 2), (i / 2), 1, 1);
		v[i].color		= COLOR(1, 1, 1, 1);
		v[i].texcoord	= VECTOR2(0, 0);
	}

	primitiveType_	= Wrapper::PRIMITIVE::TYPE::TRIANGLE_STRIP;
	primitiveNum_	= 2;

	if (manager_)
	{
		if (const auto& systems = manager_->GetSystems())
		{
			if (const auto& graphics = systems->GetGraphics())
			{
				if (const auto& wrapper = graphics->GetWrapper())
				{
					buffer_ = wrapper->CreateVertexBuffer(v, sizeof(VERTEX2D), 4);
				}
			}
		}
	}
}

bool CanvasRenderer::Image::Animation(float add)
{
	pattern_ += add;	
	if (pattern_ >= split_.x * split_.y)
	{
		pattern_ = 0;
		return true;
	}
	return false;
}
