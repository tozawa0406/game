//-----------------------------------------------------------------------------
//
//	3D`æĮ[ObjectRendererManager.h]
//	Auther : ËāVãÄū
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _OBJECT_RENDERER_MANAGER_H_
#define _OBJECT_RENDERER_MANAGER_H_

#include "../../Define/Define.h"
#include "../BaseManager.h"

struct ObjectRenderer;
//-----------------------------------------------------------------------------
//	NXéū
//-----------------------------------------------------------------------------
class ObjectRendererManager : public BaseManager<ObjectRenderer>
{
	friend ObjectRenderer;
public:
	ObjectRendererManager(Systems* systems) : BaseManager(systems) {}

	HRESULT Init(void) override { return S_OK; }
	void	FastDraw(void);
	void    Draw(void) override;	//|S`æ
	void	DrawShadow(void);

private:
	void Add(ObjectRenderer* obj);
	void Sort(void);

};


#endif // _OBJECT_RENDERER_MANAGER_H_