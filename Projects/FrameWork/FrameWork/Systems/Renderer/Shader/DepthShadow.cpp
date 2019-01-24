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
#include "../../../Graphics/DirectX11/Dx11Wrapper.h"

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

	fileName_ = shaderDirectoryName + "DepthBufShadow.hlsl";
	vVersion_ = "vs_5_0";
	pVersion_ = "ps_5_0";

	if (FAILED(Shader::Init())) { return E_FAIL; }

	constantBuffer_.emplace_back(dev_->CreateConstantBuffer(sizeof(CONSTANT)));

	return S_OK;
}

HRESULT DepthShadow::SetParam(const MATRIX& mtxWorld, const COLOR& color, VECTOR4 texcoord) const
{
	const auto& systems = manager_->GetSystems();

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

	string temp = "";
	int size[7] = { sizeof(MATRIX),sizeof(MATRIX), sizeof(MATRIX), sizeof(VECTOR4), sizeof(VECTOR4), sizeof(MATRIX), sizeof(MATRIX) };
	dev_->SetShaderValue(constantBuffer_[0], 7, &temp, size, &cbuf);

	return S_OK;
}
