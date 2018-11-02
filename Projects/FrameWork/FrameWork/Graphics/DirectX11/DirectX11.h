//-----------------------------------------------------------------------------
//
//	DirectX11の処理[DirectX11.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DIREXT_X11_H_
#define _DIRECT_X11_H_

#include "WICTextureLoader.h"
#include "../Graphics.h"

#pragma warning (disable : 4005)
#pragma warning (disable : 4838)
#include <xnamath.h>
#include <d3d11.h>
#include <d3dx11.h>

#pragma comment(lib, "d3d11.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#endif

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class DirectX11 : public Graphics
{
	friend Windows;
public:
	ID3D11Device*        GetDevice(void)        { return pDevice_;        }		// デバイスの受け渡し
	ID3D11DeviceContext* GetDeviceContext(void) { return pDeviceContext_; }		// コンテキストの受け渡し
	IDXGISwapChain*		 GetSwapChain(void)		{ return pSwapChain_;     }

private:
	DirectX11(Windows* window);

	HRESULT Init(void)      override;		// 初期化処理
	void    Uninit(void)    override;		// 後処理
	HRESULT DrawBegin(void) override;		// 描画開始
	void    DrawEnd(void)   override;		// 描画終了

	void ClearRenderer(void) override;		// 画面のクリア

	HRESULT InitAll(void);			// 全ての初期化処理
	bool    SetDevice(void);		// デバイスの設定

	IDXGISwapChain*         pSwapChain_;
	ID3D11Device*           pDevice_;				// DirectXの機能
	ID3D11DeviceContext*    pDeviceContext_;		// GPUの方
	ID3D11RenderTargetView* pRenderTargetView_;
	ID3D11DepthStencilView* pDepthStencilView_;
};

#endif // _DIRECT_X11_H