/*
 * @file		Dx11RenderTarget.h
 * @brief		DirectX11のレンダーターゲットの処理
 * @author		戸澤翔太
 * @data		2018/12/05
 */
#ifndef _DX11_RENDERER_TARGET_H_
#define _DX11_RENDERER_TARGET_H_

#include "Dx11Include.h"
#include "../RenderTarget.h"
#include "Dx11Utility/CascadeManager.h"

class Dx11RenderTarget : public RenderTarget
{
public:
	~Dx11RenderTarget(void);
	void Uninit(void) override;

	void ClearRendererTarget(List num, COLOR color) override;

	void BeginMultiRendererTarget(void) override;
	void EndMultiRendererTarget(void)	override;

	void Draw(List num, VECTOR2 position, VECTOR2 size) override;

	void CreateScreenshot(const string& filename) override;

	void BeginDrawShadow(void) override;
	void EndDrawShadow(void)   override;

	CascadeManager* GetCascadeManager(void) { return cascade_; }

	inline ID3D11ShaderResourceView*	GetShaderResourceView(List num) { return shaderResourceView_[static_cast<int>(num)]; }
	inline ID3D11SamplerState*			GetShadowSampler(void)			{ return shadowSampler_; }

private:
	friend class DirectX11;

	Dx11RenderTarget(DirectX11* dx11);
	HRESULT Init(void) override;

	HRESULT CreateNormalRenderTarget(List num);
	HRESULT CreateShadowmapRenderTarget(void);
	HRESULT CreateRenderTarget(List num, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srDesc);

	ID3D11RenderTargetView*     renderTargetView_[static_cast<int>(List::MAX)];
	ID3D11ShaderResourceView*   shaderResourceView_[static_cast<int>(List::MAX)];
	ID3D11DepthStencilView*		depthStencilView_;
	ID3D11DepthStencilView*		shadowDepthStencilView_;
	ID3D11SamplerState*			shadowSampler_;
	D3D11_VIEWPORT				shadowViewport_;

	DirectX11*		directX11_;
	CascadeManager* cascade_;

};

#endif // _DX11_RENDERER_TARGET_H