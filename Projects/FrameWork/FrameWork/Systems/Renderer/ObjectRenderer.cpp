//-----------------------------------------------------------------------------
//
//	3D•`‰æ[ObjectRenderer.cpp]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(RendererType t) : systems_(nullptr), transform_(nullptr), fastDraw(false), enable(true)
											   , type(t), shadow(true), shader(Shader::ENUM::UNKOUWN), sort(true)
{
	material.diffuse  = COLOR(1, 1,  1,  1);
	material.ambient  = COLOR(0.3f, 0.3f, 0.3f, 1);
	material.emission = COLOR(0, 0, 0, 0);
	material.specular = COLOR(0, 0, 0, 0);
	material.power    = 0;
}

ObjectRenderer::~ObjectRenderer(void)
{
	if (systems_) { systems_->GetObjectRenderer()->Remove(this); }
}

void ObjectRenderer::Init(Systems* s, const Transform* t)
{
	systems_   = s;
	transform_ = t;
	systems_->GetObjectRenderer()->Add(this);
}
