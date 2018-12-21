//-----------------------------------------------------------------------------
//
//	グラフィックスAPI[Graphics.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "../Define/Define.h"
#include "Wrapper.h"
#include "RenderTarget.h"

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class Graphics
{
	friend class Windows;
	friend class Systems;

public:	
	// グラフィックスのタイプ
	enum class Type
	{
		UNKNOWN  = -1,
		DirectX9 = 0,
		DirectX11,
		OpenGL,
		MAX
	};	

	// 解像度
	static constexpr int WIDTH  = 1280;
	static constexpr int HEIGHT = 720;

	inline Windows*			GetWindow(void)			{ return window_;  }		// ウィンドウクラスの受け渡し
	inline Wrapper*			GetWrapper(void)		{ return wrapper_; }		// ラッパークラスの受け渡し
	inline RenderTarget*	GetRenderTarget(void)	{ return renderTarget_; }

	virtual void	Uninit(void) = 0;	// 後処理
protected:
	Graphics(Windows* window) { window_ = window; }		// コンストラクタ

	virtual HRESULT Init(void)      = 0;	// 初期化処理
	virtual HRESULT DrawBegin(void)	= 0;	// 描画開始
	virtual void	DrawEnd(void)	= 0;	// 描画終了

	Windows*		window_;		// ウィンドウクラスへのポインタ
	Wrapper*		wrapper_;		// ラッパークラス
	RenderTarget*	renderTarget_;	// レンダーターゲットクラス
};

#endif // _GRAPHICS_H