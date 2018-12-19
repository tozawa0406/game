#include "CascadeManager.h"
#include "../../../Systems/GameSystems.h"
#include "../../DirectX11/Dx11Wrapper.h"
#include "../../../Windows/Windows.h"
#include "../../../Systems/Light.h"

#include "../../../Scene/SceneManager.h"
#include "../../../../../Sources/Scene/GameScene.h"
#include "../../../Systems/Camera/CameraManager.h"

static const int MAP_SIZE = 1024 * 2;

CascadeManager::CascadeManager(void) : 
	ConstantDraw_(nullptr)
	, isSoft_(true)
	, isCascadeColor_(false)
{
}

CascadeManager::~CascadeManager(void)
{
}

HRESULT CascadeManager::InitShadowState(DirectX11* directX)
{
	const auto& device = directX->GetDevice();

    HRESULT hr = S_OK;

	for (int i = 0; i < MAX_CASCADE; ++i)
	{
		ID3D11Texture2D* pDepthTex;
		// 深度テクスチャの生成
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));

		texDesc.Width				= MAP_SIZE;
		texDesc.Height				= MAP_SIZE;
		texDesc.Format				= DXGI_FORMAT_R16_TYPELESS;
		texDesc.MipLevels			= 1;
		texDesc.ArraySize			= 1;
		texDesc.SampleDesc.Count	= 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage				= D3D11_USAGE_DEFAULT;
		texDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags		= 0;
		texDesc.MiscFlags			= 0;

		hr = device->CreateTexture2D(&texDesc, nullptr, &pDepthTex);
		if (FAILED(hr)) { return hr; }


		// 深度ステンシルビューの生成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(dsvDesc));
		dsvDesc.Format				= DXGI_FORMAT_D16_UNORM;
		dsvDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(pDepthTex, &dsvDesc, &shadowState_.pDSV[i]);
		if (FAILED(hr)) { return hr; }


		// 深度用シェーダリソースビューの生成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format						= DXGI_FORMAT_R16_UNORM;
		srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels			= 1;
		srvDesc.Texture2D.MostDetailedMip	= 0;

		hr = device->CreateShaderResourceView(pDepthTex, &srvDesc, &shadowState_.pDepthSRV[i]);
		if (FAILED(hr)) { return hr; }

		ReleasePtr(pDepthTex);
	}

    // ビューポートの設定
    shadowState_.viewport.Width    = static_cast<float>(MAP_SIZE);
    shadowState_.viewport.Height   = static_cast<float>(MAP_SIZE);
    shadowState_.viewport.TopLeftX = 0.0f;
    shadowState_.viewport.TopLeftY = 0.0f;
    shadowState_.viewport.MinDepth = 0.0f;
	shadowState_.viewport.MaxDepth = 1.0f;

	// 頂点シェーダの生成
    ID3DBlob* pBlob;
	ID3DBlob* pErrors = nullptr;
	hr = D3DX11CompileFromFileA("./Resource/Data/ShadowVS.hlsl", NULL, NULL, "VSFunc", "vs_4_0", 0, 0, NULL, &pBlob, &pErrors, NULL);
	if (FAILED(hr))
	{
		OutputDebugStringA((const char*)pErrors->GetBufferPointer());
		__debugbreak();

		return hr; 
	}
	hr = device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &shadowState_.pVS);
    pBlob->Release();
	if (FAILED(hr)) { return hr; }

    // 定数バッファの生成
    D3D11_BUFFER_DESC bDesc;
	ZeroMemory(&bDesc, sizeof(bDesc));
	bDesc.Usage          = D3D11_USAGE_DEFAULT;
	bDesc.ByteWidth      = sizeof( CONSTANT_SHADOW );
	bDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = 0;

	hr = device->CreateBuffer(&bDesc, nullptr, &shadowState_.pCB);
	if (FAILED(hr)) { return hr; }

    // サンプラーステートの生成
    D3D11_SAMPLER_DESC sDesc;
	ZeroMemory(&sDesc, sizeof(sDesc));
	sDesc.AddressU       = D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.AddressV       = D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.AddressW       = D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.BorderColor[0] = 1.0f;
	sDesc.BorderColor[1] = 1.0f;
	sDesc.BorderColor[2] = 1.0f;
	sDesc.BorderColor[3] = 1.0f;
	sDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	sDesc.Filter         = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sDesc.MaxAnisotropy  = 1;
	sDesc.MipLODBias     = 0;
	sDesc.MinLOD         = -FLT_MAX;
	sDesc.MaxLOD         = +FLT_MAX;

    // サンプラーステートを生成
	hr = device->CreateSamplerState(&sDesc, &shadowState_.pSmp);
	if (FAILED(hr)) { return hr; }

    return hr;
}

void CascadeManager::UninitShadowState(void)
{
	for (auto& p : shadowState_.pDSV) { ReleasePtr(p); }
	for (auto& p : shadowState_.pDepthSRV) { ReleasePtr(p); }
	ReleasePtr(shadowState_.pVS);
	ReleasePtr(shadowState_.pCB);
	ReleasePtr(shadowState_.pSmp);
}

HRESULT CascadeManager::Init(DirectX11* directX)
{
	const auto& device = directX->GetDevice();

	HRESULT hr = InitShadowState(directX);
	if (FAILED(hr)) { return hr; }

	// 定数バッファの生成
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage			= D3D11_USAGE_DEFAULT;
	desc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth		= sizeof(CONSTANT_DRAW);
	desc.CPUAccessFlags = 0;

	hr = device->CreateBuffer(&desc, nullptr, &ConstantDraw_);
	if (FAILED(hr)) { return hr; }

	return hr;
}

void CascadeManager::Uninit(void)
{
	ReleasePtr(ConstantDraw_);

    UninitShadowState();
}

void CascadeManager::Begin(int i)
{
	// カスケード
	ComputeShadowMatrixPSSM();

	const auto& directX11 = static_cast<DirectX11*>(Systems::Instance()->GetRenderer());
	const auto& context = directX11->GetDeviceContext();

	context->RSSetViewports(1, &shadowState_.viewport);

	ID3D11RenderTargetView* pRTV = nullptr;
	context->OMSetRenderTargets(1, &pRTV, shadowState_.pDSV[i]);

	context->ClearDepthStencilView(shadowState_.pDSV[i], D3D11_CLEAR_DEPTH, 1.0f, 0);
	drawNum_ = i;
}

void CascadeManager::SetShadow(const MATRIX& m)
{
	const auto& directX11 = static_cast<DirectX11*>(Systems::Instance()->GetRenderer());
	const auto& context = directX11->GetDeviceContext();

	context->VSSetShader(shadowState_.pVS, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->DSSetShader(nullptr, nullptr, 0);
	context->HSSetShader(nullptr, nullptr, 0);

	CONSTANT_SHADOW param;
	param.world		= m;
	param.viewProj	= shadowMatrix_[drawNum_];

	context->UpdateSubresource(shadowState_.pCB, 0, nullptr, &param, 0, 0);
	context->VSSetConstantBuffers(0, 1, &shadowState_.pCB);
}

void CascadeManager::End(void)
{
	const auto& directX11 = static_cast<DirectX11*>(Systems::Instance()->GetRenderer());
	const auto& context = directX11->GetDeviceContext();

	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->DSSetShader(nullptr, nullptr, 0);
	context->HSSetShader(nullptr, nullptr, 0);
}

void CascadeManager::Set(void)
{
	const auto& directX11 = static_cast<DirectX11*>(Systems::Instance()->GetRenderer());
	const auto& context = directX11->GetDeviceContext();

	//　定数バッファ更新.
	CONSTANT_DRAW cbParam;
	const auto& c		= cameraPosition_;
	cbParam.cameraPos	= VECTOR4(c.x, c.y, c.z, 1);
	const auto& t			= lightDirection_;
	cbParam.lightDirection_ = VECTOR4(t.x, t.y, t.z, 1);

	// ソフトシャドウの使用
	if (isSoft_)
	{
		// テクセルサイズ
		cbParam.texlSize.x = 1.0f / MAP_SIZE;
		cbParam.texlSize.y = 1.0f / MAP_SIZE;
	}
	else
	{
		// テクセルサイズが0なら計算しても無意味
		cbParam.texlSize.x = 0;
		cbParam.texlSize.y = 0;
	}
	// カスケードの色処理
	cbParam.texlSize.z = (isCascadeColor_) ? 1.0f : 0.0f;
	cbParam.texlSize.w = 0;

	for (int i = 0; i < 4; ++i)
	{
		cbParam.shadow[i]   = shadowMatrix_[i];
		cbParam.splitPos[i] = splitShadowMap_[i].z;
		cbParam.splitPosXMax[i] = splitShadowMap_[i].xMax;
		cbParam.splitPosXMin[i] = splitShadowMap_[i].xMin;
	}
	context->UpdateSubresource(ConstantDraw_, 0, nullptr, &cbParam, 0, 0);

	context->VSSetConstantBuffers(2, 1, &ConstantDraw_);
	context->PSSetShaderResources(3, 4, shadowState_.pDepthSRV);

	context->PSSetSamplers(3, 1, &shadowState_.pSmp);
}

void CascadeManager::ComputeShadowMatrixPSSM(void)
{
	Camera* camera = nullptr;
	LightInfo info;
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& sceneManager = systems->GetSceneManager())
		{
			if (sceneManager->GetSceneNum() == SceneList::GAME)
			{
				if (const auto& game = static_cast<GameScene*>(sceneManager->GetScene()))
				{
					if (const auto& light = game->GetLight())
					{
						info = light->GetLightInfo();
					}
					else { return; }
				}
				else { return; }
				if (const auto& cameraManager = sceneManager->GetCameraManager())
				{
					camera = cameraManager->GetCamera();
				}
			}
			else { return; }
		}
	}
	cameraPosition_ = camera->GetPos();

	// ライトの方向ベクトル
	lightDirection_ = VecNorm(info.direction);

	float diff = 265 - 60;
	float oneSize = diff / MAX_CASCADE;

	float nearFar[MAX_CASCADE + 1];
	for (int i = 0; i < MAX_CASCADE + 1; ++i)
	{
		nearFar[i] = i * oneSize;
	}
	float oneAria = 200;
	for (int j = 0; j < MAX_CASCADE; ++j)
	{
		float aria = oneAria * (1 + j);
		// 凸包
		VECTOR3 convexHull[8];
		convexHull[0] = VECTOR3(-aria, -aria, -aria);
		convexHull[1] = VECTOR3( aria, -aria, -aria);
		convexHull[2] = VECTOR3(-aria, -aria,  aria);
		convexHull[3] = VECTOR3( aria, -aria,  aria);
		convexHull[4] = VECTOR3(-aria,  aria, -aria);
		convexHull[5] = VECTOR3( aria,  aria, -aria);
		convexHull[6] = VECTOR3(-aria,  aria,  aria);
		convexHull[7] = VECTOR3( aria,  aria,  aria);

		// カメラの行列を求める
		VECTOR3 pos = camera->GetPos();
		VECTOR3 dir = camera->GetFront();
		Transform t;
		t.position = camera->GetPos();
		t.rotation.x = atan2f(pos.z - dir.z, pos.y - dir.y);
		t.rotation.y = atan2f(pos.z - dir.z, pos.x - dir.x);
		t.rotation.z = atan2f(pos.x - dir.x, pos.y - dir.y);
		t.scale = VECTOR3(1);

		splitShadowMap_[j].xMin =  1000;
		splitShadowMap_[j].xMax = -1000;
		MATRIX world = MATRIX().Identity().Scaling(0.25f).Create(&t);
		VECTOR2 second = VECTOR2(1000, -1000);
		for (int i = 0; i < 8; ++i)
		{
			convexHull[i] = VecTransform(convexHull[i], world);
			if (convexHull[i].x < splitShadowMap_[j].xMin) { splitShadowMap_[j].xMin = convexHull[i].x; }
			if (convexHull[i].x > splitShadowMap_[j].xMax) { splitShadowMap_[j].xMax = convexHull[i].x; }
		}

		// ライトのビュー行列を算出
		MATRIX lightView = CreateLookTo(info.position, lightDirection_, VECTOR3(0, 1, 0));

		// 求め直したライトのビュー行列を使ってAABBを求める
		VECTOR3 point = VecTransformCoord(convexHull[0], lightView);
		VECTOR3 mini = point;
		VECTOR3 maxi = point;
		for (int i = 1; i < 8; ++i)
		{
			point = VecTransformCoord(convexHull[i], lightView);
			mini = VecMin(mini, point);
			maxi = VecMax(maxi, point);
		}

		// サイズを求める
		float size = VecLength((maxi - mini));
		// ライトの射影行列
		MATRIX lightProj  = CreateOrthographic(size, size, nearFar[j], nearFar[j + 1]);
		splitShadowMap_[j].z = nearFar[j + 1];

		// ライトのビュー射影行列を求める
		MATRIX lightViewProj = lightView * lightProj;

		point = VecTransformCoord(convexHull[0], lightViewProj);
		mini = point;
		maxi = point;
		for (int i = 1; i < 8; ++i)
		{
			point = VecTransformCoord(convexHull[i], lightViewProj);
			mini = VecMin(mini, point);
			maxi = VecMax(maxi, point);
		}
		BOX box;
		box.mini = mini;
		box.maxi = maxi;
		// クリップ行列を求める
		MATRIX clip = CreateUnitCubeClipMatrix(box);

		// シャドウマップめいっぱいに映るように
		lightProj = lightProj * clip;

		shadowMatrix_[j] = lightView * lightProj;
	}
}

MATRIX CascadeManager::CreateUnitCubeClipMatrix(const BOX& box)
{
	// 単位キューブクリップ行列を求める
	MATRIX clip;
	clip._11 = 2.0f / (box.maxi.x - box.mini.x);
	clip._12 = 0.0f;
	clip._13 = 0.0f;
	clip._14 = 0.0f;

	clip._21 = 0.0f;
	clip._22 = 2.0f / (box.maxi.y - box.mini.y);
	clip._23 = 0.0f;
	clip._24 = 0.0f;

	clip._31 = 0.0f;
	clip._32 = 0.0f;
	clip._33 = 1.0f / (box.maxi.z - box.mini.z);
	clip._34 = 0.0f;

	clip._41 = -(box.maxi.x + box.mini.x) / (box.maxi.x - box.mini.x);
	clip._42 = -(box.maxi.y + box.mini.y) / (box.maxi.y - box.mini.y);
	clip._43 = -box.mini.z / (box.maxi.z - box.mini.z);
	clip._44 = 1.0f;

	return clip;
}

VECTOR3 CascadeManager::VecMin(const VECTOR3& v1, const VECTOR3& v2)
{
	return VECTOR3(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z));
}

VECTOR3 CascadeManager::VecMax(const VECTOR3& v1, const VECTOR3& v2)
{
	return VECTOR3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
}

MATRIX CascadeManager::CreateLookTo(const VECTOR3& cameraPosition, const VECTOR3& cameraDirection, const VECTOR3& cameraUpVector)
{
	MATRIX result;
	VECTOR3 zaxis = -VecNorm(cameraDirection);
	VECTOR3 xaxis = VecNorm(VecCross(cameraUpVector, zaxis));
	VECTOR3 yaxis = VecNorm(VecCross(zaxis, xaxis));

	result._11 = xaxis.x;
	result._12 = yaxis.x;
	result._13 = zaxis.x;
	result._14 = 0.0f;

	result._21 = xaxis.y;
	result._22 = yaxis.y;
	result._23 = zaxis.y;
	result._24 = 0.0f;

	result._31 = xaxis.z;
	result._32 = yaxis.z;
	result._33 = zaxis.z;
	result._34 = 0.0f;

	result._41 = -VecDot(xaxis, cameraPosition);
	result._42 = -VecDot(yaxis, cameraPosition);
	result._43 = -VecDot(zaxis, cameraPosition);
	result._44 = 1.0f;

	return result;
}

MATRIX CascadeManager::CreateOrthographic(const float width, const float height, const float nearClip, const float farClip)
{
	assert(width != 0.0f);
	assert(height != 0.0f);
	register float diffNF = nearClip - farClip;
	assert(diffNF != 0.0f);

	MATRIX result;
	result._11 = 2.0f / width;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 2.0f / height;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = 1.0f / diffNF;
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = nearClip / diffNF;
	result._44 = 1.0f;

	return result;
}

void CascadeManager::DrawShadowMap(void)
{
	if (!Systems::Instance()->GetDebug()->GetDebug()) { return; }
	const auto& directX11 = static_cast<DirectX11*>(Systems::Instance()->GetRenderer());
	const auto& context = directX11->GetDeviceContext();
	const auto& wrapper = directX11->GetWrapper();

	int i = 0;
	for (auto& srv : shadowState_.pDepthSRV)
	{
		context->PSSetShaderResources(0, 1, &srv);
		wrapper->DrawQuad(VECTOR2(10.0f + 100 + (i * 210), Windows::HEIGHT - 10 - 100.0f), VECTOR2(200, 200));
		i++;
	}
}

void CascadeManager::GuiUpdate(void)
{
	if (ImGui::Button("Color")) { isCascadeColor_ = !isCascadeColor_; }
	ImGui::SameLine();
	if (isCascadeColor_) { ImGui::Text("true"); }
	else { ImGui::Text("false"); }

	if (ImGui::Button("Soft")) { isSoft_ = !isSoft_; }
	ImGui::SameLine();
	if (isSoft_) { ImGui::Text("true"); }
	else { ImGui::Text("false"); }
}
