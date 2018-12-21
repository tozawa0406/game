//-----------------------------------------------------------------------------
//
//	シェーダー[Shader.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Shader.h"
#include "ShaderManager.h"
#include "../../GameSystems.h"
#include "../../../Windows/Windows.h"

#include "../../../Graphics/DirectX11/Dx11Wrapper.h"

HRESULT Shader::Init(void)
{
	dev_ = manager_->GetSystems()->GetGraphics()->GetWrapper();
	if (dev_ == nullptr) { return E_FAIL; }
	if (vMethod_ != "")
	{
		vertexShader_ = dev_->CreateVertexShader(fileName_, vMethod_, vVersion_, layout_, layoutSize_);
		if (vertexShader_ == Wrapper::R_ERROR) { return E_FAIL; }
	}
	else
	{
		vertexShader_ = Wrapper::S_NULL;
	}

	if (pMethod_ != "")
	{
		pixelShader_ = dev_->CreatePixelShader(fileName_, pMethod_, pVersion_);
		if (pixelShader_ == Wrapper::R_ERROR) { return E_FAIL; }
	}
	else
	{
		pixelShader_ = Wrapper::S_NULL;
	}

	return S_OK;
}
