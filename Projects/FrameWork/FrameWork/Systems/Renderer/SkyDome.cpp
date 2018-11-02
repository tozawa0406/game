//-----------------------------------------------------------------------------
//
//	�X�J�C�h�[��[SkyDome.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "SkyDome.h"

//! @def	�_�𓮂������x
static constexpr float MOVE = 0.0001f;

SkyDome::SkyDome(Systems* systems)
{
	systems_ = systems;

	transform_.position = VECTOR3(0, 0, 0);
	transform_.rotation = VECTOR3(0, 0, 0);
	transform_.scale    = VECTOR3(2, 2, 2);

	mesh_.Init(systems, (int)Model::Game::SKY, &transform_);
	mesh_.sort   = false;
	mesh_.shadow = false;
	mesh_.shader = Shader::ENUM::UNKOUWN;
	mesh_.fastDraw = true;
}

void SkyDome::Update(void)
{
	transform_.rotation.y += MOVE;
}
