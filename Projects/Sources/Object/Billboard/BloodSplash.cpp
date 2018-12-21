#include "BloodSplash.h"

//! @def	大きさ
static const VECTOR3 SCALE = VECTOR3(2, 10, 1);
//! @def	消えるタイミング(フレーム)
static constexpr int DESTROY = 30;

BloodSplash::BloodSplash(VECTOR3 position, float rotation) : Object(ObjectTag::BILLBOARD)
	, cnt_(0)
{
	transform_.position		= position;
	transform_.scale		= SCALE;
	transform_.rotation.z	= rotation;
}

BloodSplash::~BloodSplash(void)
{
}

void BloodSplash::Init(void)
{
	renderer_.Init(Systems::Instance(), static_cast<int>(Texture::Game::SLASH), &transform_);
	renderer_.billbord	= true;
	renderer_.shadow	= false;
	renderer_.shader	= Shader::ENUM::BILLBOARD;
}

void BloodSplash::Uninit(void)
{
}

void BloodSplash::Update(void)
{
	cnt_++;
	if (cnt_ > DESTROY)
	{
		Destroy();
	}
}
