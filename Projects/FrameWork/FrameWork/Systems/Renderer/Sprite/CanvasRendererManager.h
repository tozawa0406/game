//-----------------------------------------------------------------------------
//
//	2D`æĮ[CanvasRendererManager.h]
//	Auther : ËāVãÄū
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _CANVAS_RENDERER_MANAGER_H_
#define _CANVAS_RENDERER_MANAGER_H_

#include "../../../Define/Define.h"
#include "../../BaseManager.h"

struct CanvasRenderer;
//-----------------------------------------------------------------------------
//	NXéū
//-----------------------------------------------------------------------------
class CanvasRendererManager : public BaseManager<CanvasRenderer>
{
	friend struct CanvasRenderer;
public:
	CanvasRendererManager(Systems* systems) : BaseManager(systems) {}

	HRESULT Init(void) override { return S_OK; }
	void    Draw(void) override;	//|S`æ

private:
	void Add(CanvasRenderer* obj);
	void Sort(void);
};

#endif // _CANVAS_RENDERER_MANAGER_H_
