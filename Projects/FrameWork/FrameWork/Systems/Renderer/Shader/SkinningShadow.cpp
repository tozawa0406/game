//-----------------------------------------------------------------------------
//
//	デフォルトシェーダー[Default.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "SkinningShadow.h"
#include "ShaderManager.h"
#include "../../GameSystems.h"
#include "../../../Windows/Windows.h"
#include "../../../Scene/SceneManager.h"
#include "../../Camera/CameraManager.h"
#include "ZTexture.h"

#include "../../../Graphics/DirectX11/Dx11Wrapper.h"
#include "Default.h"

HRESULT SkinningShadowShader::Init(void)
{
	fileName_ = shaderDirectoryName + "SkinningShadow.hlsl";

	vMethod_  = "VS_Main";
	vVersion_ = "vs_5_0";

	pMethod_  = "PS_Main";
	pVersion_ = "ps_5_0";

	if (FAILED(Shader::Init())) { return E_FAIL; }

	constantBuffer_.emplace_back(dev_->CreateConstantBuffer(sizeof(CONSTANT)));

	return S_OK;
}

HRESULT SkinningShadowShader::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(color);
	UNREFERENCED_PARAMETER(texcoord);


	CONSTANT cbuf;
	memcpy_s(&cbuf.world, sizeof(MATRIX), &mtx, sizeof(MATRIX));
	cbuf.world._44 = 1;
	const ZTexture* depth = (ZTexture*)manager_->GetShader(Shader::ENUM::ZTEXTURE);
	memcpy_s(&cbuf.view , sizeof(MATRIX), &depth->GetLightView(), sizeof(MATRIX));
	memcpy_s(&cbuf.proj , sizeof(MATRIX), &depth->GetLightProj(), sizeof(MATRIX));

	string temp = "";
	int size[3] = { sizeof(MATRIX),sizeof(MATRIX), sizeof(MATRIX) };
	dev_->SetShaderValue(constantBuffer_[0], 3, &temp, size, &cbuf);
	dev_->SetConstantBuffer(Wrapper::ShaderType::Vertex, 0, 1, constantBuffer_[0]);

	return S_OK;
}
