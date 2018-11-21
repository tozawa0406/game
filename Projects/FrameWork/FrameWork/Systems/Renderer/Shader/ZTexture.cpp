//-----------------------------------------------------------------------------
//
//	Zテクスチャシェーダー[ZTexture.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "ZTexture.h"
#include "../../GameSystems.h"
#include "../../../Windows/Windows.h"
#include "../../Camera/CameraManager.h"
#include "../../Light.h"
#include "../../../../../Sources/Scene/GameScene.h"

#include "../../../Graphics/DirectX11/DirectX11Wrapper.h"
#include "Default.h"

ZTexture::ZTexture(ShaderManager* manager) : Shader(manager, shaderDirectoryName + FILENAME)
{
}

ZTexture::~ZTexture(void)
{
	manager_->GetSystems()->GetRenderer()->GetWrapper()->ReleaseShadowMap();
}

HRESULT ZTexture::Init(void)
{
	vMethod_ = "VS_ZBufferCalc";
	pMethod_ = "PS_ZBufferPlot";

	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		fileName_ = shaderDirectoryName + "ZValue.hlsl";

		vVersion_ = "vs_5_0";
		pVersion_ = "ps_5_0";

		//頂点インプットレイアウトを定義	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		layout_ = &layout[0];
		layoutSize_ = sizeof(layout) / sizeof(layout[0]);

		if (FAILED(Shader::Init())) { return E_FAIL; }

		const auto& dx11 = (DirectX11Wrapper*)dev_;
		constantBuffer_.emplace_back(dx11->CreateConstantBuffer(sizeof(CONSTANT)));
	}
	else
	{
		if (FAILED(Shader::Init())) { return E_FAIL; }
	}

	return manager_->GetSystems()->GetRenderer()->GetWrapper()->CreateShadowMap();
}

HRESULT ZTexture::BeginDraw(void)
{
	const auto& dev = manager_->GetSystems()->GetRenderer()->GetWrapper();

	const auto& camera = manager_->GetSystems()->GetSceneManager()->GetCameraManager()->GetCamera();
	VECTOR3 at = camera->GetAt();
	VECTOR3 up = camera->GetUp();

	const auto& sceneManager = manager_->GetSystems()->GetSceneManager();
	if (sceneManager->GetSceneNum() == SceneList::GAME)
	{
		const auto& scene = sceneManager->GetScene();
		if (scene)
		{
			const auto& light = ((GameScene*)scene)->GetLight();

			if (light)
			{
				view_ = dev->CreateViewMatrix(light->GetLightInfo().position, light->GetLightInfo().at, up);
				proj_ = dev->CreateProjectionMatrix(Camera::FOV, (float)Graphics::WIDTH / Graphics::HEIGHT, 60, 265);
			}
		}
	}
	dev->BeginDrawShadow();

	return S_OK;
}

HRESULT ZTexture::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(color);

	const auto& dev = manager_->GetSystems()->GetRenderer()->GetWrapper();

	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX9)
	{
	}
	else if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		const auto& dx11 = (DirectX11Wrapper*)dev;

		CONSTANT cbuf;
		cbuf.world = mtx;
		cbuf.world._44 = 1;
		cbuf.view  = view_;
		cbuf.proj  = proj_;
		cbuf.texcoord = texcoord;

		const auto& context  = dx11->GetContext();
		const auto& constant = dx11->GetConstantBuffer(constantBuffer_[0]);
		context->UpdateSubresource(constant, 0, nullptr, &cbuf, 0, 0);
	}

	return S_OK;
}

HRESULT ZTexture::EndDraw(void)
{
	manager_->GetSystems()->GetRenderer()->GetWrapper()->EndDrawShadow();

	return S_OK;
}
