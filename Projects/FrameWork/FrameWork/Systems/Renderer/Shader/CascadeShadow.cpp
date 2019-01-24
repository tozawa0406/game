//-----------------------------------------------------------------------------
//
//	Zテクスチャシェーダー[CascadeShadow.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "CascadeShadow.h"
#include "../../GameSystems.h"
#include "../../../Windows/Windows.h"
#include "../../Camera/CameraManager.h"
#include "../../Light.h"

#include "../../../Graphics/DirectX11/Dx11Wrapper.h"
#include "../../../Graphics/DirectX11/Dx11RenderTarget.h"
#include "Default.h"

CascadeShadow::CascadeShadow(ShaderManager* manager) : Shader(manager, shaderDirectoryName + FILENAME)
	, cascade_(nullptr)
	, drawNum_(0)
{
}

CascadeShadow::~CascadeShadow(void)
{
}

HRESULT CascadeShadow::Init(void)
{
	vMethod_ = "VSFunc";
	pMethod_ = "";

	fileName_ = shaderDirectoryName + "ShadowVS.hlsl";

	vVersion_ = "vs_5_0";
	pVersion_ = "ps_5_0";

	if (FAILED(Shader::Init())) { return E_FAIL; }

	constantBuffer_.emplace_back(dev_->CreateConstantBuffer(sizeof(CONSTANT)));
	constantBuffer_.emplace_back(dev_->CreateConstantBuffer(sizeof(CONSTANT_DRAW)));

	const auto& graphics = manager_->GetSystems()->GetGraphics();
	const auto& renderTarget = graphics->GetRenderTarget();

	cascade_ = renderTarget->GetCascadeManager();

	return S_OK;
}

HRESULT CascadeShadow::BeginDraw(int i)
{
	const auto& graphics = manager_->GetSystems()->GetGraphics();
	const auto& renderTarget = static_cast<Dx11RenderTarget*>(graphics->GetRenderTarget());

	renderTarget->BeginDrawShadow(i);
	drawNum_ = i;

	return S_OK;
}

HRESULT CascadeShadow::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(texcoord);
	UNREFERENCED_PARAMETER(color);

	CONSTANT cbuf;
	cbuf.world = mtx;
	cbuf.world._44 = 1;
	cbuf.shadowViewProj = cascade_->GetShadowMatrix(drawNum_);

	string temp = "";
	int size[2] = { sizeof(MATRIX),sizeof(MATRIX) };		
	dev_->SetShaderValue(constantBuffer_[0], 2, &temp, size, &cbuf);

	return S_OK;
}

HRESULT CascadeShadow::EndDraw(void)
{
	manager_->GetSystems()->GetGraphics()->GetRenderTarget()->EndDrawShadow();

	//　定数バッファ更新.
	CONSTANT_DRAW cbuf;
	const auto& c = cascade_->GetCameraPosition();
	cbuf.cameraPos = VECTOR4(c.x, c.y, c.z, 1);
	const auto& t = cascade_->GetLightDirection();
	cbuf.lightDirection_ = VECTOR4(t.x, t.y, t.z, 1);

	// ソフトシャドウの使用
	if (cascade_->IsSoft())
	{
		// テクセルサイズ
		cbuf.texlSize.x = 1.0f / CascadeManager::MAP_SIZE;
		cbuf.texlSize.y = 1.0f / CascadeManager::MAP_SIZE;
	}
	else
	{
		// テクセルサイズが0なら計算しても無意味
		cbuf.texlSize.x = 0;
		cbuf.texlSize.y = 0;
	}
	// カスケードの色処理
	cbuf.texlSize.z = (cascade_->IsCascadeColor()) ? 1.0f : 0.0f;
	cbuf.texlSize.w = 0;

	for (int i = 0; i < 4; ++i)
	{
		cbuf.shadow[i] = cascade_->GetShadowMatrix(i);
		const auto& split = cascade_->GetShadowSplit(i);
		cbuf.splitPos[i] = split.z;
		cbuf.splitPosXMax[i] = split.xMax;
		cbuf.splitPosXMin[i] = split.xMin;
	}

	string temp = "";
	int splitSize = sizeof(float) * CascadeManager::MAX_CASCADE;
	int size[7] = { sizeof(VECTOR4),sizeof(VECTOR4),sizeof(VECTOR4), splitSize,splitSize, splitSize, sizeof(MATRIX) * CascadeManager::MAX_CASCADE };
	dev_->SetShaderValue(constantBuffer_[1], 7, &temp, size, &cbuf);
	dev_->SetConstantBuffer(Wrapper::ShaderType::Vertex, 2, 1, constantBuffer_[1]);

	return S_OK;
}
