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
#include "../../../Graphics/DirectX11/Dx11Wrapper.h"

HRESULT PointSpriteShader::Init(void)
{
	fileName_ = shaderDirectoryName + "PointSprite.hlsl";

	vMethod_ = "VS_Main";
	vVersion_ = "vs_5_0";

	pMethod_ = "PS_Main";
	pVersion_ = "ps_5_0";


	// ifを抜けるとReleaseでインプットレイアウトがエラー吐く
	if (FAILED(Shader::Init())) { return E_FAIL; }

	const auto& dx11 = (Dx11Wrapper*)dev_;
	constantBuffer_.emplace_back(dx11->CreateConstantBuffer(sizeof(CONSTANT)));
	dx11->CreateGeometryShader(fileName_, "GS_Main", "gs_5_0");

	return S_OK;
}

HRESULT PointSpriteShader::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(texcoord);
	const auto& systems = manager_->GetSystems();

	CONSTANT cbuf;
	cbuf.world = mtx;
	cbuf.view = systems->GetSceneManager()->GetCameraManager()->GetView();
	cbuf.proj = systems->GetSceneManager()->GetCameraManager()->GetProj();

	VECTOR4 t = { color.r, color.g, color.b, color.a };
	cbuf.diffuse = t;


	string temp = "";
	int size[4] = { sizeof(MATRIX),sizeof(MATRIX), sizeof(MATRIX), sizeof(VECTOR4) };
	dev_->SetShaderValue(constantBuffer_[0], 4, &temp, size, &cbuf);

	return S_OK;
}
