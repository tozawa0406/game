#ifndef _SCREENSHOT_H_
#define _SCREENSHOT_H_

#include "../Define/Define.h"
#include "BaseManager.h"
#include "Renderer/Sprite/CanvasRenderer.h"

class Screenshot : public Interface
{
	static constexpr int BUF_SIZE = 15;
public:
	Screenshot(Systems* systems);
	~Screenshot(void) override;

	HRESULT Init(void)   override;
	void    Update(void) override;
	void    Draw(void)   override;

private:
	void CreateTexture(void);
	void UIUpdate(void);

	// スクリーンショットUI用
	CanvasRenderer sprite_[2];
	int screenUICnt_;				// UI描画の時間
	bool enable_;
};

#endif // _SCREENSHOT_H_