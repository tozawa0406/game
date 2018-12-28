/*
 * @file		ClearFailed.h
 * @brief		¬Œ÷¸”sUIˆ—
 * @author		ŒËàVãÄ‘¾
 * @data		2018/12/27
 */
#ifndef _CLEAR_FAILED_H_
#define _CLEAR_FAILED_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/Renderer/CanvasRenderer/CanvasRendererImage.h> 
#include <FrameWork/Object/Object.h>

class ClearFailed : public Object
{
public:
	ClearFailed();
	~ClearFailed();

	void Init(void)	  override;
	void Uninit(void) override;
	void Update(void) override;

	inline void SetEnable(bool enable)	{ img_.SetEnable(enable);	}
	inline void SetColor(COLOR color)	{ img_.SetColor(color);		}
	inline void SetPattern(int pattern) { img_.SetPattern(static_cast<float>(pattern)); }

private:
	CanvasRenderer::Image img_;

	int  cnt_;			// ƒJƒEƒ“ƒ^
};

#endif // _CLEAR_FAILED_H_
