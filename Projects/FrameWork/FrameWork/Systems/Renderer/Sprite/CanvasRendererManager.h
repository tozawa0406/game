//-----------------------------------------------------------------------------
//
//	2D•`‰æŠÇ—[CanvasRendererManager.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _CANVAS_RENDERER_MANAGER_H_
#define _CANVAS_RENDERER_MANAGER_H_

#include "../../../Define/Define.h"
#include "../../BaseManager.h"

struct CanvasRenderer;
//-----------------------------------------------------------------------------
//	ƒNƒ‰ƒXéŒ¾
//-----------------------------------------------------------------------------
class CanvasRendererManager : public BaseManager<CanvasRenderer>
{
	friend struct CanvasRenderer;
public:
	CanvasRendererManager(Systems* systems) : BaseManager(systems) {}

	HRESULT Init(void) override { return S_OK; }
	void    Draw(void) override;	//ƒ|ƒŠƒSƒ“•`‰æˆ—

private:
	void Add(CanvasRenderer* obj);
	void Sort(void);
};

#endif // _CANVAS_RENDERER_MANAGER_H_
