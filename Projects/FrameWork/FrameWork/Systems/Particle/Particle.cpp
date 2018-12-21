//-----------------------------------------------------------------------------
//
//	パーティクル[Particle.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Particle.h"
#include "ParticleManager.h"

Particle::Particle(ParticleManager* manager, PARTICLE_DATA data) : texNum_((int)Texture::Base::WHITE), destroy_(false), frame_(0), data_(data)
																 , vertexBuffer_(0), manager_(manager), systems_(manager->GetSystems())
{
	transform_.position = data.vertex.position;
	data.vertex.position = VECTOR3(0, 0, 0);

	a_ = data.vertex.color.a / data.limit;

	const auto& dev = systems_->GetGraphics()->GetWrapper();

	vertexBuffer_ = dev->CreateVertexBuffer(&data.vertex, sizeof(data.vertex), 1);

	manager->Add(this);
}

Particle::~Particle(void)
{
	systems_->GetGraphics()->GetWrapper()->ReleaseBuffer(vertexBuffer_, Wrapper::FVF::VERTEX_POINT);
}

void Particle::Update(void)
{
	data_.vertex.color.a -= a_;

	transform_.position += data_.velocity;
	frame_++;
	if (frame_ > data_.limit) { this->Destroy(); }
}
