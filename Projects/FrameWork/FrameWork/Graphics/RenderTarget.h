#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include "../Define/Define.h"

class RenderTarget
{
public:
	/* @enum	レンダーターゲットの種類	*/
	enum class List : uint8
	{
		DEFAULT = 0,
		SCREEN_SHOT,
		COLOR,
		POSITION,
		NORMAL,
		SHADOW,
		MAX
	};

	//! @def	描画の大きさ
	static constexpr float SIZE_X = 240;
	static constexpr float SIZE_Y = 135;

	RenderTarget(void) : debugDraw_(List::MAX) {}
	virtual ~RenderTarget(void) {}

	virtual HRESULT Init(void)		= 0;
	virtual void	Uninit(void)	= 0;

	virtual void ClearRendererTarget(List num, COLOR color) = 0;

	virtual void BeginMultiRendererTarget(void) = 0;
	virtual void EndMultiRendererTarget(void)	= 0;

	virtual void Draw(List num, VECTOR2 position, VECTOR2 size) = 0;

	virtual void CreateScreenshot(const string& filename) = 0;

	virtual void BeginDrawShadow(void) = 0;
	virtual void EndDrawShadow(void)   = 0;

	inline void SetDebugDraw(List num) { debugDraw_ = num; }

protected:
	List debugDraw_;
};

#endif // _RENDER_TARGET_H_