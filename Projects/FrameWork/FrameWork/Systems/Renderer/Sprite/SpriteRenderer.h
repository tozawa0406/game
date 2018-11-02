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
	void	SetVertex(UINT n, UINT vnum)  { vertexBuffer = n; vertexNum = vnum; }
	void	SetIndex(UINT n, UINT vnum)   { indexBuffer = n; indexNum = vnum;   }
	UINT	GetVertexBuffer(void)	const { return vertexBuffer; }
	UINT	GetIndexBuffer(void)	const { return indexBuffer;  }
	UINT	GetVertexNum(void)		const { return vertexNum;    }
	UINT	GetIndexNum(void)		const { return indexNum;     }
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
	UINT	vertexBuffer;
	UINT	indexBuffer;
	UINT	vertexNum;
	UINT	indexNum;
	VECTOR4 texcoord;
};

#endif // _CANVAS_RENDERER_H_