//-----------------------------------------------------------------------------
//
//	ìñÇΩÇËîªíËï`âÊ[ColliderRenderer.h]
//	Auther : åÀ‡V„ƒëæ
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DRAW_COLLIDER_H_
#define _DRAW_COLLIDER_H_

#include "../../Define/Define.h"
#include "../../Graphics/Wrapper.h"
#include "../BaseManager.h"

//-----------------------------------------------------------------------------
//	ÉNÉâÉXêÈåæ
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
	const MATRIX*	 GetTransMtx(void)  const { return transMtx;  }
	const VECTOR3&	 GetOffsetPosition(void) const { return offsetPosition; }
	const VECTOR3&	 GetOffsetRotation(void) const { return offsetRotation; }
	const VECTOR3&	 GetSize(void)		const { return size; }
	const COLOR&	 GetColor(void)     const { return color; }
	void			 SetColor(COLOR c)	      { color = c; }
	uint GetVertexBuffer(void) const { return vertexBuffer; }
	uint GetVertexNum(void)	   const { return vnum;    }
	uint GetPrimitiveNum(void) const { return pnum;         }
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
	VECTOR3		offsetPosition;
	VECTOR3		offsetRotation;
	const MATRIX* parentMtx;
	const MATRIX* transMtx;
	COLOR		color;

	Wrapper::PRIMITIVE::TYPE type;
	byte		pnum;
	uint		vertexBuffer;
	uint		vnum;

	Systems*	systems;
};

class ColliderRendererManager : public BaseManager<ColliderRenderer>
{
	friend struct ColliderRenderer;
	friend class Systems;
public:
	ColliderRendererManager(Systems* systems) : BaseManager(systems) {}
	HRESULT Init(void) override { return S_OK; }
	void    Draw(void) override;	//É|ÉäÉSÉìï`âÊèàóù
};

#endif // _POLYGON_H_
