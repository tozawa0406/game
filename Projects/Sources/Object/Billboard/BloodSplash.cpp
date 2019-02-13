#include "BloodSplash.h"

//! @def	�傫��
static const VECTOR3 SCALE = VECTOR3(1.5f, 50, 1);
//! @def	������^�C�~���O(�t���[��)
static constexpr int DESTROY = 30;

BloodSplash::BloodSplash(VECTOR3 position, float rotation) : Particle(Systems::Instance()->GetParticleManager(), PARTICLE_DATA(position, 1, DESTROY))
{
	transform_.scale		= SCALE;
	transform_.rotation.z	= rotation;
	texNum_ = static_cast<int>(Resources::Texture::Camp::SLASH);
}

BloodSplash::~BloodSplash(void)
{
}
