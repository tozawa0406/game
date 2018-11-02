//-----------------------------------------------------------------------------
//
//	3D•`‰æŠÇ—[ObjectRendererManager.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _OBJECT_RENDERER_MANAGER_H_
#define _OBJECT_RENDERER_MANAGER_H_

#include "../../Define/Define.h"
#include "../BaseManager.h"

struct ObjectRenderer;
//-----------------------------------------------------------------------------
//	ƒNƒ‰ƒXéŒ¾
//-----------------------------------------------------------------------------
class ObjectRendererManager : public BaseManager<ObjectRenderer>
{
	friend ObjectRenderer;
public:
	ObjectRendererManager(Systems* systems) : BaseManager(systems) {}

	HRESULT Init(void) override { return S_OK; }
	void	FastDraw(void);
	void    Draw(void) override;	//ƒ|ƒŠƒSƒ“•`‰æˆ—
	void	DrawShadow(void);

private:
	void Add(ObjectRenderer* obj);
	void Sort(void);
};


#endif // _OBJECT_RENDERER_MANAGER_H_