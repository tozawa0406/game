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

#include "../../../Graphics/DirectX11/Dx11Wrapper.h"
#include "Default.h"

ZTexture::ZTexture(ShaderManager* manager) : Shader(manager, shaderDirectoryName + FILENAME)
{
}

ZTexture::~ZTexture(void)
{
}

HRESULT ZTexture::Init(void)
{
	vMethod_ = "VS_ZBufferCalc";
	pMethod_ = "PS_ZBufferPlot";

	fileName_ = shaderDirectoryName + "ZValue.hlsl";

	vVersion_ = "vs_5_0";
	pVersion_ = "ps_5_0";

	if (FAILED(Shader::Init())) { return E_FAIL; }

	constantBuffer_.emplace_back(dev_->CreateConstantBuffer(sizeof(CONSTANT)));

	return S_OK;
}

HRESULT ZTexture::BeginDraw(void)
{
	const auto& graphics = manager_->GetSystems()->GetGraphics();

	const auto& camera = manager_->GetSystems()->GetSceneManager()->GetCameraManager()->GetCamera();
	VECTOR3 at = camera->GetAt();
	VECTOR3 up = camera->GetUp();

	const auto& sceneManager = manager_->GetSystems()->GetSceneManager();
	const auto& sceneNum = sceneManager->GetSceneNum();
	if (sceneNum == SceneList::CAMP || sceneNum == SceneList::BUTTLE)
	{
		const auto& scene = sceneManager->GetScene();
		if (scene)
		{
			const auto& light = scene->GetLight();

			if (light)
			{
				view_ = CreateViewMatrix(light->GetLightInfo().position, light->GetLightInfo().at, up);
				proj_ = CreateProjectionMatrix(Camera::FOV, Windows::WIDTH / Windows::HEIGHT, 60, 350);
			}
		}
	}
	graphics->GetRenderTarget()->BeginDrawShadow(0);

	return S_OK;
}

HRESULT ZTexture::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(color);

	CONSTANT cbuf;
	cbuf.world = mtx;
	cbuf.world._44 = 1;
	cbuf.view  = view_;
	cbuf.proj  = proj_;
	cbuf.texcoord = texcoord;

	string temp = "";
	int size[4] = { sizeof(MATRIX),sizeof(MATRIX), sizeof(MATRIX), sizeof(VECTOR4) };
	dev_->SetShaderValue(constantBuffer_[0], 4, &temp, size, &cbuf);

	return S_OK;
}

HRESULT ZTexture::EndDraw(void)
{
	manager_->GetSystems()->GetGraphics()->GetRenderTarget()->EndDrawShadow();

	return S_OK;
}
