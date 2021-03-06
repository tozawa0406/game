//-----------------------------------------------------------------------------
//
//	XJCh[[SkyDome.h]
//	Auther : ËāVãÄū
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "SkyDome.h"
#include <FrameWork/Scene/SceneManager.h>

//! @def	_ðŪĐ·Žx
static constexpr float MOVE = 0.0002f;

SkyDome::SkyDome(Systems* systems)
{
	systems_ = systems;

	transform_.position = VECTOR3(0, 0, 0);
	transform_.rotation = VECTOR3(0, 0, 0);
	transform_.scale    = VECTOR3(5, 5, 5);

	mesh_.Init(systems, static_cast<int>(Resources::Model::Base::SKY), &transform_);
	mesh_.sort   = false;
	mesh_.shadow = false;
	mesh_.shader = Shader::ENUM::UNKOUWN;
	mesh_.fastDraw = true;
}

void SkyDome::Update(void)
{
	transform_.rotation.y += MOVE;
	//auto c = mesh_.material.diffuse;
	//c.r = max(c.r - 0.001f, 0.15f);
	//c.g = max(c.g - 0.0015f, 0.15f);
	//c.b = max(c.b - 0.002f, 0.15f);
	//mesh_.material.diffuse = c;
}
