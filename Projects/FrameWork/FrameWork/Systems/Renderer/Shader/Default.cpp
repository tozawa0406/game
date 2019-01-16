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
	const auto& systems = manager_->GetSystems();
	const auto& window = systems->GetWindow();
	if (window->GetGraphicsType() == Graphics::Type::DirectX11)
	{
		fileName_ = shaderDirectoryName + "Animation.hlsl";

		vMethod_  = "VS_Main";
		vVersion_ = "vs_5_0";

		pMethod_  = "PS_Main";
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

		const auto& dx11 = (Dx11Wrapper*)dev_;
		constantBuffer_.emplace_back(dx11->CreateConstantBuffer(sizeof(CONSTANT)));
		constantBuffer_.emplace_back(dx11->CreateConstantBuffer(sizeof(CONSTANT_BONE)));
	}
	else
	{
		if (FAILED(Shader::Init())) { return E_FAIL; }
	}

	return S_OK;
}

HRESULT DefaultShader::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	const auto& systems = manager_->GetSystems();
	const auto& dev = systems->GetGraphics()->GetWrapper();
	const auto& window = systems->GetWindow();
	const auto& type = window->GetGraphicsType();
	if (type == Graphics::Type::DirectX9)
	{
	}
	else if (type == Graphics::Type::DirectX11)
	{
		const auto& dev11 = ((Dx11Wrapper*)dev);

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
	
		const auto& context = dev11->GetContext();
		const auto& constant = dev11->GetConstantBuffer(constantBuffer_[0]);
		context->UpdateSubresource(constant, 0, NULL, &cbuf, 0, 0);
	}

	return S_OK;
}
