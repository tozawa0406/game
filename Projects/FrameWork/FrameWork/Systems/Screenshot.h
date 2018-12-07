/*
 * @file		Screenshot.h
 * @brief		スクリーンショットの管理
 * @author		戸澤翔太
 * @data		2018/12/05
 */
#ifndef _SCREENSHOT_H_
#define _SCREENSHOT_H_

#include "../Define/Define.h"
#include "BaseManager.h"
#include "../Graphics/RenderTarget.h"
#include "../Graphics/Wrapper.h"

class Screenshot : public Interface
{
public:
	Screenshot(Systems* systems);
	~Screenshot(void) override;

	HRESULT Init(void)   override;
	void    Update(void) override;
	void    Draw(void)   override;

private:
	void CreateTexture(void);

	int				screenUICnt_;		//! UI描画の時間
	RenderTarget*	renderTarget_;		//! レンダーターゲットへのポインタ
	Wrapper*		wrapper_;			//! 描画用のラッパーへのポインタ
};

#endif // _SCREENSHOT_H_