//-----------------------------------------------------------------------------
//
//	”Âƒ|ƒŠ•`‰æ[SpriteRenderer.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include "../ObjectRenderer.h"

struct SpriteRenderer : public ObjectRenderer
{
	void	Init(Systems* systems, int texNu, const Transform* transform);
	void	SetVertex(uint n, uint vnum)  { vertexBuffer = n; vertexNum = vnum; }
	void	SetIndex(uint n, uint vnum)   { indexBuffer = n; indexNum = vnum;   }
	uint	GetVertexBuffer(void)	const { return vertexBuffer; }
	uint	GetIndexBuffer(void)	const { return indexBuffer;  }
	uint	GetVertexNum(void)		const { return vertexNum;    }
	uint	GetIndexNum(void)		const { return indexNum;     }
	VECTOR4 GetTexcoord(void)		const { return texcoord;	 }

	int			  texNum;

	float		  pattern;
	VECTOR2		  split;

	bool billbord;
	bool xBillbord;

	SpriteRenderer(void);
	~SpriteRenderer(void);

	bool Animation(float add) override;

private:
	uint	vertexBuffer;
	uint	indexBuffer;
	uint	vertexNum;
	uint	indexNum;
	VECTOR4 texcoord;
};

#endif // _CANVAS_RENDERER_H_