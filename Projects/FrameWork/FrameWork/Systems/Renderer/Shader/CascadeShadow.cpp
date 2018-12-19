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
#include "../../../../../Sources/Scene/GameScene.h"

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

	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		fileName_ = shaderDirectoryName + "ShadowVS.hlsl";

		vVersion_ = "vs_5_0";
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

		if (FAILED(Shader::Init())) { return E_FAIL; }

		const auto& dx11 = (Dx11Wrapper*)dev_;
		constantBuffer_.emplace_back(dx11->CreateConstantBuffer(sizeof(CONSTANT)));
	}
	else
	{
		if (FAILED(Shader::Init())) { return E_FAIL; }
	}

	const auto& graphics = manager_->GetSystems()->GetRenderer();
	const auto& renderTarget = static_cast<Dx11RenderTarget*>(graphics->GetRenderTarget());

	cascade_ = renderTarget->GetCascadeManager();

	return S_OK;
}

HRESULT CascadeShadow::BeginDraw(int i)
{
	const auto& graphics = manager_->GetSystems()->GetRenderer();
	const auto& renderTarget = static_cast<Dx11RenderTarget*>(graphics->GetRenderTarget());

	renderTarget->BeginDrawShadow();
	cascade_->Begin(i);
	drawNum_ = i;

	return S_OK;
}

HRESULT CascadeShadow::SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const
{
	UNREFERENCED_PARAMETER(texcoord);
	UNREFERENCED_PARAMETER(color);

	const auto& dev = manager_->GetSystems()->GetRenderer()->GetWrapper();

	if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX9)
	{
	}
	else if (Windows::GRAPHICS_TYPE == Graphics::Type::DirectX11)
	{
		const auto& dx11 = (Dx11Wrapper*)dev;

		CONSTANT cbuf;
		cbuf.world = mtx;
		cbuf.world._44 = 1;
		cbuf.shadowViewProj = cascade_->GetShadowMatrix(drawNum_);

		const auto& context  = dx11->GetContext();
		const auto& constant = dx11->GetConstantBuffer(constantBuffer_[0]);
		context->UpdateSubresource(constant, 0, nullptr, &cbuf, 0, 0);
	}

	return S_OK;
}

HRESULT CascadeShadow::EndDraw(void)
{
	cascade_->End();
	manager_->GetSystems()->GetRenderer()->GetRenderTarget()->EndDrawShadow();

	return S_OK;
}
