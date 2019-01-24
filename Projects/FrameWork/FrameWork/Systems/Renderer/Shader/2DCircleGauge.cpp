//-----------------------------------------------------------------------------
//
//	Auther : ŒËàVãÄ‘¾
//-----------------------------------------------------------------------------
#include "2DCircleGauge.h"
#include "ShaderManager.h"
#include "../../GameSystems.h"
#include "../../../Windows/Windows.h"
#include "../../../Graphics/DirectX11/Dx11Wrapper.h"

HRESULT CircleGaugeShader::Init(void)
{
	vMethod_ = "VS_Main";
	vVersion_ = "vs_5_0";

	pMethod_ = "PS_Main";
	pVersion_ = "ps_5_0";

	if (FAILED(Shader::Init())) { return E_FAIL; }

	constantBuffer_.emplace_back(dev_->CreateConstantBuffer(sizeof(CONSTANT)));

	return S_OK;
}

HRESULT CircleGaugeShader::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(color);
	UNREFERENCED_PARAMETER(texcoord);

	CONSTANT cbuf;
	cbuf.screen.x = Windows::WIDTH;
	cbuf.screen.y = Windows::HEIGHT;
	cbuf.screen.z = -3.14f + (6.28f - ( mtx._11 * 0.0628f));
	cbuf.screen.w = 0;

	string temp = "";
	int size = sizeof(VECTOR4);
	dev_->SetShaderValue(constantBuffer_[0], 1, &temp, &size, &cbuf);
	dev_->SetConstantBuffer(Wrapper::ShaderType::Vertex, 0, 1, constantBuffer_[0]);
	dev_->SetConstantBuffer(Wrapper::ShaderType::Pixel, 0, 1, constantBuffer_[0]);

	return S_OK;
}
