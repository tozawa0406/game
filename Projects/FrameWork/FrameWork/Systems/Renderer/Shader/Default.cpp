//-----------------------------------------------------------------------------
//
//	デフォルトシェーダー[Default.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Default.h"
#include "ShaderManager.h"
#include "../../GameSystems.h"
#include "../../../Windows/Windows.h"
#include "../../../Scene/SceneManager.h"
#include "../../Camera/CameraManager.h"
#include "ZTexture.h"

#include "../../../Graphics/DirectX11/Dx11Wrapper.h"
#include "../../Light.h"

HRESULT DefaultShader::Init(void)
{
	fileName_ = shaderDirectoryName + "Animation.hlsl";

	vMethod_  = "VS_Main";
	vVersion_ = "vs_5_0";

	pMethod_  = "PS_Main";
	pVersion_ = "ps_5_0";

	if (FAILED(Shader::Init())) { return E_FAIL; }

	constantBuffer_.emplace_back(dev_->CreateConstantBuffer(sizeof(CONSTANT)));
	constantBuffer_.emplace_back(dev_->CreateConstantBuffer(sizeof(CONSTANT_BONE)));

	return S_OK;
}

HRESULT DefaultShader::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	const auto& systems = manager_->GetSystems();

	CONSTANT cbuf;
	memcpy_s(&cbuf.world, sizeof(MATRIX), &mtx, sizeof(MATRIX));
	cbuf.world._44 = 1;
	memcpy_s(&cbuf.view , sizeof(MATRIX), &systems->GetSceneManager()->GetCameraManager()->GetView(), sizeof(MATRIX));
	memcpy_s(&cbuf.proj , sizeof(MATRIX), &systems->GetSceneManager()->GetCameraManager()->GetProj(), sizeof(MATRIX));

	cbuf.texcoord = texcoord;
	VECTOR4 t = { color.r, color.g, 1, 1 };
	cbuf.diffuse = t;

	const auto& scene = systems->GetSceneManager()->GetScene();
	const auto& lp = scene->GetLight()->GetLightInfo().position;
	cbuf.lightPosition = VECTOR4(lp.x, lp.y, lp.z, 1);

	const ZTexture* depth = (ZTexture*)manager_->GetShader(Shader::ENUM::ZTEXTURE);
	cbuf.lightView = depth->GetLightView();
	cbuf.lightProj = depth->GetLightProj();
	
	string temp = "";
	int size[8] = { sizeof(MATRIX),sizeof(MATRIX), sizeof(MATRIX), sizeof(VECTOR4), sizeof(VECTOR4), sizeof(VECTOR4), sizeof(MATRIX), sizeof(MATRIX) };
	dev_->SetShaderValue(constantBuffer_[0], 8, &temp, size, &cbuf);

	return S_OK;
}
