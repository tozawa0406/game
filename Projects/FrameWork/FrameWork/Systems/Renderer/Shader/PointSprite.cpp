//-----------------------------------------------------------------------------
//
//	ポイントスプライトシェーダー[PointSprite.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "PointSprite.h"
#include "ShaderManager.h"
#include "../../GameSystems.h"
#include "../../../Scene/SceneManager.h"
#include "../../Camera/CameraManager.h"

#include "../../../Windows/Windows.h"
#include "../../../Graphics/DirectX11/DirectX11Wrapper.h"

HRESULT PointSpriteShader::Init(void)
{
	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		fileName_ = shaderDirectoryName + "PointSprite.hlsl";

		vMethod_ = "VS_Main";
		vVersion_ = "vs_5_0";

		pMethod_ = "PS_Main";
		pVersion_ = "ps_5_0";

		//頂点インプットレイアウトを定義	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PSIZE"   , 0, DXGI_FORMAT_R32_FLOAT         , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		layout_ = &layout[0];
		layoutSize_ = sizeof(layout) / sizeof(layout[0]);

		// ifを抜けるとReleaseでインプットレイアウトがエラー吐く
		if (FAILED(Shader::Init())) { return E_FAIL; }

		const auto& dx11 = (DirectX11Wrapper*)dev_;
		constantBuffer_.emplace_back(dx11->CreateConstantBuffer(sizeof(CONSTANT)));
		dx11->CreateGeometryShader(fileName_, "GS_Main", "gs_5_0");
	}
	else
	{
		if (FAILED(Shader::Init())) { return E_FAIL; }
	}

	return S_OK;
}

HRESULT PointSpriteShader::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(texcoord);
	Systems* systems = manager_->GetSystems();
	const auto& dev = systems->GetRenderer()->GetWrapper();
	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX9)
	{
	}
	else if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		const auto& dev11 = ((DirectX11Wrapper*)dev);

		CONSTANT cbuf;
		cbuf.world = mtx;
		cbuf.view = systems->GetSceneManager()->GetCameraManager()->GetView();
		cbuf.proj = systems->GetSceneManager()->GetCameraManager()->GetProj();

		VECTOR4 t = { color.r, color.g, color.b, color.a };
		cbuf.diffuse = t;

		const auto& context = dev11->GetContext();
		const auto& constant = dev11->GetConstantBuffer(constantBuffer_[0]);
		context->UpdateSubresource(constant, 0, NULL, &cbuf, 0, 0);
	}

	return S_OK;
}
