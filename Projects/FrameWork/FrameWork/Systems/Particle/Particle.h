//-----------------------------------------------------------------------------
//
//	パーティクル[Particle.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "../../Define/Define.h"
#include "../GameSystems.h"

struct PARTICLE_DATA
{
	VERTEXPOINT vertex;
	VECTOR3     velocity;
	int         limit;

	PARTICLE_DATA(void) {}
	PARTICLE_DATA(VECTOR3 position, float size = 1, int limit = 0, COLOR color = COLOR(1, 1, 1, 1), VECTOR3 velocity = VECTOR3(0, 0, 0))
	{
		this->vertex.position	= position;
		this->vertex.size		= size;
		this->vertex.color		= color;
		this->limit				= limit;
		this->velocity			= velocity;
	}
};

class Particle
{
	friend class ParticleManager;
public:
	Particle(ParticleManager* manager, PARTICLE_DATA data);
	~Particle(void);

	virtual void Update(void);
	void SetTexture(int texNum) { texNum_ = texNum; }

	const Transform& GetTransform(void)    const { return transform_; }
	int				 GetTexNum(void)       const { return texNum_;    }
	uint			 GetVertexBuffer(void) const { return vertexBuffer_; }

	COLOR			 GetColor(void)		   const { return data_.vertex.color; }

private:
	void Destroy(void) { destroy_ = true; }

	bool			destroy_;
	int				frame_;
	PARTICLE_DATA	data_;
	uint			vertexBuffer_;
	float			a_;

	ParticleManager* manager_;
	Systems*		 systems_;

protected:
	Transform		transform_;
	int				texNum_;
};

#endif // _EFFECT_H_
