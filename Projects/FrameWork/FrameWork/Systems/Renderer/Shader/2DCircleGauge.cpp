//-----------------------------------------------------------------------------
//
//	Auther : 戸澤翔太
//-----------------------------------------------------------------------------
#include "2DCircleGauge.h"
#include "ShaderManager.h"
#include "../../GameSystems.h"
#include "../../../Windows/Windows.h"
#include "../../../Graphics/DirectX11/Dx11Wrapper.h"

HRESULT CircleGaugeShader::Init(void)
{
	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		vMethod_ = "VS_Main";
		vVersion_ = "vs_5_0";

		pMethod_ = "PS_Main";
		pVersion_ = "ps_5_0";

		//頂点インプットレイアウトを定義	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		layout_ = &layout[0];
		layoutSize_ = sizeof(layout) / sizeof(layout[0]);

		if (FAILED(Shader::Init())) { return E_FAIL; }

		const auto& dx11 = (Dx11Wrapper*)dev_;
		constantBuffer_.emplace_back(dx11->CreateConstantBuffer(sizeof(CONSTANT)));
	}
	else
	{
		if (FAILED(Shader::Init())) { return E_FAIL; }
	}

	return S_OK;
}

HRESULT CircleGaugeShader::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(color);
	UNREFERENCED_PARAMETER(texcoord);
	Systems* systems = manager_->GetSystems();

	const auto& dev = systems->GetRenderer()->GetWrapper();
	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		const auto& dev11 = ((Dx11Wrapper*)dev);

		CONSTANT cbuf;
		cbuf.screen.x = (float)Graphics::WIDTH;
		cbuf.screen.y = (float)Graphics::HEIGHT;
		cbuf.screen.z = -3.14f + (6.28f - ( mtx._11 * 0.0628f));
		cbuf.screen.w = 0;

		const auto& context = dev11->GetContext();
		const auto& constant = dev11->GetConstantBuffer(constantBuffer_[0]);
		context->UpdateSubresource(constant, 0, NULL, &cbuf, 0, 0);
		context->VSSetConstantBuffers(0, 1, &constant);
		context->PSSetConstantBuffers(0, 1, &constant);
	}

	return S_OK;
}
