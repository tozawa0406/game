//-----------------------------------------------------------------------------
//
//	デフォルトシェーダー[Default.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "CSSkinningRender.h"
#include "ShaderManager.h"
#include "../../GameSystems.h"
#include "../../../Windows/Windows.h"
#include "../../../Scene/SceneManager.h"
#include "../../Camera/CameraManager.h"
#include "ZTexture.h"

#include "../../../Graphics/DirectX11/Dx11Wrapper.h"

HRESULT CSSkinningRenderShader::Init(void)
{
	fileName_ = shaderDirectoryName + "CSSkinningRender.hlsl";

	vMethod_  = "VS_Main";
	vVersion_ = "vs_5_0";

	pMethod_  = "PS_Main";
	pVersion_ = "ps_5_0";
	
	// ifを抜けるとReleaseでインプットレイアウトがエラー吐く
	if (FAILED(Shader::Init())) { return E_FAIL; }

	constantBuffer_.emplace_back(dev_->CreateConstantBuffer(sizeof(CONSTANT)));

	return S_OK;
}

HRESULT CSSkinningRenderShader::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(texcoord);
	UNREFERENCED_PARAMETER(color);

	const auto& systems = manager_->GetSystems();
	CONSTANT cbuf;
	memcpy_s(&cbuf.world, sizeof(MATRIX), &mtx, sizeof(MATRIX));
	cbuf.world._44 = 1;
	memcpy_s(&cbuf.view , sizeof(MATRIX), &systems->GetSceneManager()->GetCameraManager()->GetView(), sizeof(MATRIX));
	memcpy_s(&cbuf.proj , sizeof(MATRIX), &systems->GetSceneManager()->GetCameraManager()->GetProj(), sizeof(MATRIX));

	cbuf.texcoord = VECTOR4(0, 0, 1, 1);
	VECTOR4 t = { color.r, color.g, 1, 1 };
	cbuf.diffuse = t;

	const ZTexture* depth = (ZTexture*)manager_->GetShader(Shader::ENUM::ZTEXTURE);
	cbuf.lightView = depth->GetLightView();
	cbuf.lightProj = depth->GetLightProj();
	
	string temp = "";
	int size[7] = { sizeof(MATRIX),sizeof(MATRIX), sizeof(MATRIX), sizeof(VECTOR4), sizeof(VECTOR4), sizeof(MATRIX), sizeof(MATRIX) };
	dev_->SetShaderValue(constantBuffer_[0], 7, &temp, size, &cbuf);
	dev_->SetConstantBuffer(Wrapper::ShaderType::Vertex, 0, 1, constantBuffer_[0]);

	return S_OK;
}
