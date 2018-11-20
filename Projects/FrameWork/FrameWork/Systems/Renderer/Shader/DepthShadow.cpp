//-----------------------------------------------------------------------------
//
//	深度バッファシェーダー[DepthShadow.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "DepthShadow.h"
#include "../../GameSystems.h"
#include "../../Camera/CameraManager.h"
#include "ZTexture.h"

#include "../../../Windows/Windows.h"
#include "../../../Graphics/DirectX11/DirectX11Wrapper.h"

DepthShadow::DepthShadow(ShaderManager* manager) : Shader(manager, shaderDirectoryName + FILENAME)
{
}

DepthShadow::~DepthShadow(void)
{
}

HRESULT DepthShadow::Init(void)
{
	vMethod_ = "VS_DepthBufShadow";
	pMethod_ = "PS_DepthBufShadow";

	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		fileName_ = shaderDirectoryName + "DepthBufShadow.hlsl";
		vVersion_ = "vs_5_0";
		pVersion_ = "ps_5_0";

		//頂点インプットレイアウトを定義	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		layout_ = &layout[0];
		layoutSize_ = sizeof(layout) / sizeof(layout[0]);

		// ifを抜けるとReleaseでインプットレイアウトがエラー吐く
		if (FAILED(Shader::Init())) { return E_FAIL; }

		const auto& dx11 = (DirectX11Wrapper*)dev_;
		constantBuffer_.emplace_back(dx11->CreateConstantBuffer(sizeof(CONSTANT)));
	}
	else
	{
		if (FAILED(Shader::Init())) { return E_FAIL; }
	}

	return S_OK;
}

HRESULT DepthShadow::SetParam(const MATRIX& mtxWorld, const COLOR& color, VECTOR4 texcoord) const
{
	Systems* systems = manager_->GetSystems();

	const auto& dev = systems->GetRenderer()->GetWrapper();
	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX9)
	{
	}
	else if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		const auto& dev11 = ((DirectX11Wrapper*)dev);

		CONSTANT cbuf;
		cbuf.world = mtxWorld;
		cbuf.view = systems->GetSceneManager()->GetCameraManager()->GetView();
		cbuf.proj = systems->GetSceneManager()->GetCameraManager()->GetProj();

		cbuf.texcoord = texcoord;

		VECTOR4 t = { color.r, color.g, color.b, color.a };
		cbuf.color = t;

		const ZTexture* depth = (ZTexture*)manager_->GetShader(Shader::ENUM::ZTEXTURE);
		cbuf.lightView = depth->GetLightView();
		cbuf.lightProj = depth->GetLightProj();

		const auto& context = dev11->GetContext();
		const auto& constant = dev11->GetConstantBuffer(constantBuffer_[0]);
		context->UpdateSubresource(constant, 0, NULL, &cbuf, 0, 0);
	}
	return S_OK;
}
