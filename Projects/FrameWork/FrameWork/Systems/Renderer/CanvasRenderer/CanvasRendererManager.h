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

class CanvasRendererBase;
class CanvasRendererManager : public BaseManager<CanvasRendererBase>
{
	friend CanvasRendererBase;
public:
	CanvasRendererManager(Systems* systems) : BaseManager(systems) {}

	HRESULT Init(void)	 override { return S_OK; }
	void	Uninit(void) override;
	void    Draw(void)   override;	//ƒ|ƒŠƒSƒ“•`‰æˆ—

private:
	void Add(CanvasRendererBase* obj);
	void Sort(void);
};

#endif // _CANVAS_RENDERER_MANAGER_H_
