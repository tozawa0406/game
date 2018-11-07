//-----------------------------------------------------------------------------
//
//	“–‚½‚è”»’è•`‰æ[ColliderRenderer.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DRAW_COLLIDER_H_
#define _DRAW_COLLIDER_H_

#include "../../Define/Define.h"
#include "../../Graphics/Wrapper.h"
#include "../BaseManager.h"

//-----------------------------------------------------------------------------
//	ƒNƒ‰ƒXéŒ¾
//-----------------------------------------------------------------------------
class Collider2DBase;
class Collider3DBase;
struct ColliderRenderer
{
	friend class ColliderRendererManager;
	friend class Collision2DManager;
	friend class Collision3DManager;

	ColliderRenderer(void);
	~ColliderRenderer(void);

	const Transform& GetTransform(void) const { return transform; }
	const MATRIX*	 GetParentMtx(void) const { return parentMtx; }
	const VECTOR3&	 GetOffset(void)	const { return offset; }
	const VECTOR3&	 GetSize(void)		const { return size; }
	UINT GetVertexBuffer(void) const { return vertexBuffer; }
	UINT GetVertexNum(void)	   const { return vnum;    }
	UINT GetPrimitiveNum(void) const { return pnum;         }
	Wrapper::PRIMITIVE::TYPE GetType(void) const { return type; }

	void ColliderRectangle(const Collider2DBase* col);
	void ColliderCircle(const Collider2DBase* col);

	void ColliderSegment(const Collider3DBase* col);
	void ColliderSphere(const Collider3DBase* col, bool second);

	void ColliderOBB(const Collider3DBase* col);

	void Update(const Collider2DBase* col);
	void Update(const Collider3DBase* col);

	bool		enable;	

private:
	Transform	transform;
	VECTOR3		size;
	VECTOR3		offset;
	const MATRIX* parentMtx;

	Wrapper::PRIMITIVE::TYPE type;
	BYTE		pnum;
	UINT		vertexBuffer;
	UINT		vnum;

	Systems*	systems;
};

class ColliderRendererManager : public BaseManager<ColliderRenderer>
{
	friend struct ColliderRenderer;
	friend class Systems;
public:
	ColliderRendererManager(Systems* systems) : BaseManager(systems) {}
	HRESULT Init(void) override { return S_OK; }
	void    Draw(void) override;	//ƒ|ƒŠƒSƒ“•`‰æˆ—
};

#endif // _POLYGON_H_
