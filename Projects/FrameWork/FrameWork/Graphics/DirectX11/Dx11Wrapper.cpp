//-----------------------------------------------------------------------------
//
//	DirectX11の処理[Dx11Wrapper.cpp]
//	Auther : 戸澤翔太
//
//-----------------------------------------------------------------------------
#include "Dx11Wrapper.h"
#include <locale>
#include "Dx11RenderTarget.h"
#include "../../Windows/Windows.h"

#include "../../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../../Systems/Renderer/Sprite/SpriteRenderer.h"
#include "../../Systems/Renderer/Model/MeshRenderer.h"
#include "../../Systems/Particle/Particle.h"
#include "../../Systems/Camera/Camera.h"

#include "../../Scene/SceneManager.h"
#include "../../Systems/Camera/CameraManager.h"

#include "../../Systems/Renderer/Shader/PointSprite.h"
#include "../../Systems/Renderer/Shader/Default.h"

struct SHADER_DEFAULT_SCENE
{
	MATRIX view;
	MATRIX proj;
};

struct SHADER_DEFAULT_OBJECT
{
	MATRIX  world;
	VECTOR4 texcoord;
	COLOR   diffuse;
};

struct SHADER_UI
{
	VECTOR4 position;
	VECTOR4 sizeSplit;
	VECTOR4 scale;
	VECTOR4 rotationPattern;
	COLOR color;

	SHADER_UI(void) : position(0), sizeSplit(1), scale(1), rotationPattern(0), color(1){}

	// シェーダーに渡す値のコピー
	SHADER_UI(const CanvasRenderer::Image& c)
	{
		this->position.x		= c.GetPosition().x;
		this->position.y		= c.GetPosition().y;
		this->position.z		= 1;
		this->position.w		= 1;
		this->sizeSplit.x		= c.GetSize().x;
		this->sizeSplit.y		= c.GetSize().y;
		this->scale.x			= c.GetScale().x;
		this->scale.y			= c.GetScale().y;
		this->scale.z			= c.GetScaleOffset().x;
		this->scale.w			= c.GetScaleOffset().y;
		this->rotationPattern.y = c.GetAngle();
		this->rotationPattern.z = c.GetRotationOffset().x;
		this->rotationPattern.w = c.GetRotationOffset().y;
		this->rotationPattern.x = c.GetPattern();
		this->sizeSplit.z		= c.GetSplit().x;
		this->sizeSplit.w		= c.GetSplit().y;
		this->color				= c.GetColor();
	}
};

// コンストラクタ
Dx11Wrapper::Dx11Wrapper(DirectX11* directX) : directX11_(directX), depthState_(nullptr)
{
}

HRESULT Dx11Wrapper::Init(void)
{
	const auto& pDevice = directX11_->GetDevice();
	string directoryHlsl = "";
	{
		directoryHlsl = Define::ResourceDirectoryName + "Data/UI.hlsl";
		//頂点インプットレイアウトを定義	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		CreateInputLayout(layout, sizeof(layout) / sizeof(layout[0]), directoryHlsl);
		shader_[0].vertexShader.emplace_back(this->CreateVertexShader(directoryHlsl, "VS_Main", "vs_5_0"));
		shader_[0].pixelShader.emplace_back(this->CreatePixelShader(directoryHlsl, "PS_Main", "ps_5_0"));
		shader_[0].pixelShader.emplace_back(this->CreatePixelShader(directoryHlsl, "PS_NotTexture", "ps_5_0"));
		shader_[0].constantBuffer.emplace_back(this->CreateConstantBuffer(sizeof(VECTOR4)));
		shader_[0].constantBuffer.emplace_back(this->CreateConstantBuffer(sizeof(SHADER_UI)));
	}
	{
		directoryHlsl = Define::ResourceDirectoryName + "Data/Default.hlsl";

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

		CreateInputLayout(layout, sizeof(layout) / sizeof(layout[0]), directoryHlsl);

		shader_[1].vertexShader.emplace_back(this->CreateVertexShader(directoryHlsl, "VS_Main", "vs_5_0"));
		shader_[1].vertexShader.emplace_back(this->CreateVertexShader(directoryHlsl, "VS_SpriteMain", "vs_5_0"));
		shader_[1].pixelShader.emplace_back(this->CreatePixelShader(directoryHlsl, "PS_Main", "ps_5_0"));
		shader_[1].constantBuffer.emplace_back(this->CreateConstantBuffer(sizeof(SHADER_DEFAULT_SCENE)));
		shader_[1].constantBuffer.emplace_back(this->CreateConstantBuffer(sizeof(SHADER_DEFAULT_OBJECT)));
	}
	{
		directoryHlsl = Define::ResourceDirectoryName + "Data/PointSprite.hlsl";

		//頂点インプットレイアウトを定義	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PSIZE"	, 0, DXGI_FORMAT_R32_FLOAT		   , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		CreateInputLayout(layout, sizeof(layout) / sizeof(layout[0]), directoryHlsl);

	}
	// アルファブレンドの生成
	// 加算合成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable			 = FALSE;
	BlendDesc.IndependentBlendEnable		 = FALSE;
	for(int i = 0;i < 8;++i)
	{
		BlendDesc.RenderTarget[i].BlendEnable	 = TRUE;
		BlendDesc.RenderTarget[i].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[i].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
		BlendDesc.RenderTarget[i].BlendOp        = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[i].SrcBlendAlpha  = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[i].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	pDevice->CreateBlendState(&BlendDesc, &blendState_[(int)ALFA_BREND::DEF]);

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	for (int i = 0; i < 8; ++i)
	{
		BlendDesc.RenderTarget[i].BlendEnable	 = TRUE;
		BlendDesc.RenderTarget[i].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[i].DestBlend      = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[i].BlendOp        = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[i].SrcBlendAlpha  = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[i].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	pDevice->CreateBlendState(&BlendDesc, &blendState_[(int)ALFA_BREND::ADD]);

	D3D11_DEPTH_STENCIL_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.DepthEnable = false;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	pDevice->CreateDepthStencilState(&depthDesc, &depthState_);

	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	pDevice->CreateRasterizerState(&rdc, &rasterizerState_);

	VERTEX2D v[4];
	for (int i = 0; i < 4; ++i)
	{
		v[i].position	= VECTOR4((i % 2), (i / 2), 1, 1);
		v[i].color		= COLOR(1, 1, 1, 1);
		v[i].texcoord	= VECTOR2(0, 0);
	}
	CreateVertexBuffer(v, sizeof(VERTEX2D), 4);

	font_ = new Dx11Font;
	if (font_)
	{
		font_->Init(directX11_);
	}

	return S_OK;
}

void Dx11Wrapper::Uninit(void)
{
	UninitDeletePtr(font_);

	for(int i = 0;i < (int)vertexBuffer_.size();)
	{
		ReleasePtr(vertexBuffer_[i].buffer);
		if (vertexBuffer_[i].buffer == nullptr)
		{
			auto& vb = vertexBuffer_;
			auto& thi = vertexBuffer_[i];

			for (auto itr = vb.begin(); itr != vb.end();)
			{
				if (&(*itr) == &thi)
				{
					itr = vb.erase(itr);		//配列削除
					break;
				}
				else
				{
					itr++;
				}
			}
			vb.shrink_to_fit();
		}
	}

	for (int i = 0; i < (int)ALFA_BREND::MAX; ++i)
	{
		ReleasePtr(blendState_[i]);
	}
	ReleasePtr(depthState_);
	ReleasePtr(rasterizerState_);

	for (auto& v : vertexShader_)
	{
		ReleasePtr(v);
	}
	for (auto& i : inputLayout_)
	{
		ReleasePtr(i);
	}

	for(auto& p : pixelShader_)
	{
		ReleasePtr(p.shader);
		ReleasePtr(p.sampler);
	}
	for (auto& g : geometryShader_)
	{
		ReleasePtr(g);
	}
	for (auto& c : computeShader_)
	{
		ReleasePtr(c.shader);
	}
	for (auto& c : constantBuffer_)
	{
		ReleasePtr(c);
	}
}

uint Dx11Wrapper::InsideBuffer(void)
{
	int s = (int)vertexBuffer_.size();
	for (int i = 0; i < s; ++i)
	{
		if (!vertexBuffer_[i].buffer)
		{
			return i;
		}
	}

	return R_ERROR;
}

uint Dx11Wrapper::CreateVertexBuffer(const void* v, uint size, uint vnum)
{
	VertexBuffer* temp = nullptr;
	uint inside = InsideBuffer();
	if (inside != R_ERROR) { temp = &vertexBuffer_[inside]; }
	else { temp = new VertexBuffer; }

	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;
	bd.ByteWidth		= size * vnum;
	bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags	= 0;
	bd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = v;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	ID3D11Device* pDevice = directX11_->GetDevice();
	HRESULT hr = pDevice->CreateBuffer(&bd, &data, &temp->buffer);
	if (FAILED(hr))
	{
		ReleasePtr(temp->buffer);
		return R_ERROR;
	}
	temp->stride = size;
	temp->offset = 0;

	if (inside != R_ERROR)
	{
		vertexBuffer_[inside] = *temp;
		return inside;
	}
	vertexBuffer_.emplace_back(*temp);
	delete temp;
	return (uint)vertexBuffer_.size() - 1;

}

uint Dx11Wrapper::CreateIndexBuffer(const WORD* v, uint vnum)
{
	int inside = -1;
	ID3D11Buffer* temp = nullptr;
	int s = (int)indexBuffer_.size();
	for (int i = 0; i < s; ++i)
	{
		if (indexBuffer_[i] == nullptr)
		{
			temp = indexBuffer_[i];
			inside = i;
			break;
		}
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;
	bd.ByteWidth		= sizeof(WORD) * vnum;
	bd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags	= 0;
	bd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem			= v;
	data.SysMemPitch		= 0;
	data.SysMemSlicePitch	= 0;

	ID3D11Device* pDevice = directX11_->GetDevice();
	HRESULT hr = pDevice->CreateBuffer(&bd, &data, &temp);
	if (FAILED(hr))
	{
		ReleasePtr(temp);
		return R_ERROR;
	}
	if (inside >= 0)
	{
		indexBuffer_[inside] = temp;
		return inside;
	}

	indexBuffer_.emplace_back(temp);
	return (uint)indexBuffer_.size() - 1;
}

void Dx11Wrapper::ReleaseBuffer(uint number, Wrapper::FVF fvf)
{
	if (fvf == Wrapper::FVF::INDEX)
	{
		ReleasePtr(indexBuffer_[number]);
	}
	else
	{
		ReleasePtr(vertexBuffer_[number].buffer);
	}
}

void Dx11Wrapper::SetTexture(int stage, int texNum, int modelNum)
{
	const auto& pContext = directX11_->GetDeviceContext();
	if (!pContext) { return; }

	if(texNum < 0)
	{
		ID3D11ShaderResourceView* temp = nullptr;
		pContext->PSSetShaderResources(stage, 1, &temp);
	}
	else
	{
		pContext->PSSetShaderResources(stage, 1, &texture_[modelNum + 1][texNum].data);
	}
}

void Dx11Wrapper::Draw(const CanvasRenderer::Image *obj, const Shader* shader)
{
	const auto& pContext = directX11_->GetDeviceContext();
	if (!pContext) { return; }

	int pixelNum = 0;
	// テクスチャを使用しない場合
	if (obj->GetTexNum() < (int)Resources::Texture::Base::WHITE) { pixelNum = 1; }

	ID3D11VertexShader* vertex = nullptr;
	ID3D11PixelShader*  pixel = nullptr;
	ID3D11SamplerState* sampler = nullptr;
	if (!shader)
	{
		vertex  = vertexShader_[shader_[0].vertexShader[0]];
		pixel   = pixelShader_[shader_[0].pixelShader[pixelNum]].shader;
		sampler = pixelShader_[shader_[0].pixelShader[pixelNum]].sampler;

		// テクスチャサンプラの設定
		pContext->PSSetSamplers(0, 1, &sampler);
	}
	else
	{
		vertex = vertexShader_[shader->GetVertexShader()];
		pixel  = pixelShader_[shader->GetPixelShader()].shader;
	}

	const auto& constant = constantBuffer_[shader_[0].constantBuffer[1]];
	SHADER_UI c(*obj);
	pContext->UpdateSubresource(constant, 0, NULL, &c, 0, 0);
	pContext->VSSetConstantBuffers(1, 1, &constant);

	//使用するシェーダーの設定
	pContext->VSSetShader(vertex, NULL, 0);
	pContext->PSSetShader(pixel, NULL, 0);

	// 頂点バッファをセット
	const auto& vb = vertexBuffer_[obj->GetBuffer()];
	pContext->IASetVertexBuffers(0, 1, &vb.buffer, &vb.stride, &vb.offset);

	// プリミティブトポロジーを設定
	pContext->IASetPrimitiveTopology(SelectPrimitiveType(obj->GetPrimitiveType()));
	// 描画
	pContext->Draw(obj->GetPrimitiveNum() * 2, 0);
}

void Dx11Wrapper::Draw(const SpriteRenderer* obj, const Shader* shader)
{
	const auto& pContext = directX11_->GetDeviceContext();
	if (!pContext) { return; }

	if (obj->GetShader() == Shader::ENUM::BILLBOARD)
	{
		// アルファブレンドのセット
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		pContext->OMSetBlendState(blendState_[(int)ALFA_BREND::ADD], blendFactor, 0xffffff);
	}

	ID3D11Buffer*       constant = nullptr;
	ID3D11VertexShader* vertex	 = nullptr;
	ID3D11PixelShader*  pixel	 = nullptr;
	ID3D11SamplerState* sampler  = nullptr;

	MATRIX mtx = mtx.Identity();
	if (obj->billbord) { mtx *= inverse_; }
	mtx.Create(obj->GetTransform());

	if (!shader || (shader->GetVertexShader() == 0 && shader->GetPixelShader() == 0))
	{
		constant = constantBuffer_[shader_[1].constantBuffer[1]];
		vertex   = vertexShader_[shader_[1].vertexShader[1]];
		pixel    = pixelShader_[shader_[1].pixelShader[0]].shader;
		sampler  = pixelShader_[shader_[1].pixelShader[0]].sampler;

		// コンスタントバッファの設定
		SHADER_DEFAULT_OBJECT sg;
		sg.world = mtx;
		sg.texcoord = obj->GetTexcoord();
		sg.diffuse  = obj->material.diffuse;
		pContext->UpdateSubresource(constant, 0, NULL, &sg, 0, 0);
		pContext->VSSetConstantBuffers(0, 1, &constantBuffer_[shader_[1].constantBuffer[0]]);
		pContext->VSSetConstantBuffers(1, 1, &constant);	// cbufferを使うVSに設定
	}
	else
	{
		uint vertexShaderNum = shader->GetVertexShader();
		if (vertexShaderNum == S_NULL) { vertex = nullptr; }
		else { vertex = vertexShader_[vertexShaderNum]; }

		uint pixelShaderNum = shader->GetPixelShader();
		if (pixelShaderNum == S_NULL) 
		{
			pixel = nullptr;
			sampler = nullptr;
		}
		else 
		{
			pixel = pixelShader_[pixelShaderNum].shader; 
			sampler = pixelShader_[pixelShaderNum].sampler;
		}
		constant = constantBuffer_[shader->GetConstantBuffer(0)];

		shader->SetParam(mtx, obj->material.diffuse, obj->GetTexcoord());

		pContext->VSSetConstantBuffers(0, 1, &constant);	// cbufferを使うVSに設定
	}

	// テクスチャの設定
	SetTexture(0, (int)obj->texNum);

	// テクスチャサンプラの設定
	pContext->PSSetSamplers(0, 1, &sampler);

	// シェーダーの設定
	pContext->VSSetShader(vertex, NULL, 0);
	pContext->PSSetShader(pixel, NULL, 0);
	pContext->GSSetShader(NULL, NULL, 0);

	// バッファを設定
	const auto& vb = vertexBuffer_[obj->GetVertexBuffer()];
	pContext->IASetVertexBuffers(0, 1, &vb.buffer, &vb.stride, &vb.offset);
	pContext->IASetIndexBuffer(indexBuffer_[obj->GetIndexBuffer()], DXGI_FORMAT_R16_UINT, 0);

	// プリミティブトポロジーを設定
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画
	pContext->DrawIndexed(obj->GetIndexNum(), 0, 0);

	// アルファブレンドのセット
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pContext->OMSetBlendState(blendState_[(int)ALFA_BREND::DEF], blendFactor, 0xffffffff);
}

void Dx11Wrapper::Draw(MeshRenderer* obj, const Shader* shader)
{
	const auto& pContext = directX11_->GetDeviceContext();
	if (!pContext) { return; }

	obj->Skinning();

	ID3D11Buffer*       constant = nullptr;
	ID3D11VertexShader*	vertex   = nullptr;
	ID3D11PixelShader*  pixel    = nullptr;
	ID3D11SamplerState* sampler  = nullptr;
	bool isShader = true;
	int buf = 0;
	if (!shader || shader->GetVertexShader() == 0 && shader->GetPixelShader() == 0)
	{
		vertex   = vertexShader_[shader_[1].vertexShader[0]];
		pixel    = pixelShader_[shader_[1].pixelShader[0]].shader;
		sampler  = pixelShader_[shader_[1].pixelShader[0]].sampler;
		constant = constantBuffer_[shader_[1].constantBuffer[1]];
		isShader = false;
		buf = 1;
	}
	else
	{
		uint vertexShaderNum = shader->GetVertexShader();
		if (vertexShaderNum == S_NULL) { vertex = nullptr; }
		else { vertex = vertexShader_[vertexShaderNum]; }

		uint pixelShaderNum = shader->GetPixelShader();
		if (pixelShaderNum == S_NULL)
		{
			pixel = nullptr;
			sampler = nullptr;
		}
		else
		{
			pixel = pixelShader_[pixelShaderNum].shader;
			sampler = pixelShader_[pixelShaderNum].sampler;
		}

		constant = constantBuffer_[shader->GetConstantBuffer(0)];
	}

	// コンスタントバッファの設定
	pContext->VSSetConstantBuffers(buf, 1, &constant);

	// テクスチャサンプラステートのセット
	pContext->PSSetSamplers(0, 1, &sampler);
	pContext->PSSetSamplers(2, 1, &sampler);

	int modelNum = (int)obj->GetModelNum();

	auto& temp = model_[modelNum];

	MATRIX rootMtx;
	bool rootFrameTransformMatrix = true;
	if (temp.transMtx == 0.0f) { rootFrameTransformMatrix = false; }
	else { rootMtx = temp.transMtx; }

	//プリミティブ・トポロジーをセット
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int size = (int)temp.mesh.size();
	for(int i = 0;i < size;++i)
	{
		const auto& mesh = temp.mesh[i];
		MATRIX mtx = mtx.Identity();
		// モデル行列
		if (rootFrameTransformMatrix) { mtx *= rootMtx; }
		if (mesh.transMtx != 0)  { mtx *= mesh.transMtx; }
		// ワールド行列
		mtx.Create(obj->GetTransform());
		
		// 行列の設定
		if (!isShader)
		{
			SHADER_DEFAULT_OBJECT sg;
			sg.world = mtx;
			sg.texcoord = VECTOR4(0, 0, 0, 0);
			sg.diffuse  = obj->material.diffuse;
			pContext->UpdateSubresource(constant, 0, NULL, &sg, 0, 0);
		}
		else
		{
			MATRIX tempMtx;
			memcpy_s(&tempMtx, sizeof(MATRIX), &mtx, sizeof(MATRIX));
			tempMtx._44 = (float)obj->GetModelNum();
			COLOR c = obj->material.diffuse;
			c.a = obj->GetRate();
			c.b = (float)i;
			shader->SetParam(tempMtx, c, VECTOR4(obj->GetAnimation(), obj->GetAnimationOld(), ((int)obj->GetPattern()), ((int)obj->GetPatternOld())));

			if (mesh.computeShader != R_ERROR)
			{
				//const auto& cs = computeShader_[mesh.computeShader];				
				//pContext->VSSetShaderResources(0, 1, &cs.shaderResource);
			}
		}

		// テクスチャ設定
		SetTexture(0, mesh.material.texture[0], modelNum);	
		if (mesh.material.textureName[1].size() > 0)
		{
			SetTexture(2, mesh.material.texture[1], modelNum);
		}
		// シェーダーの設定
		pContext->VSSetShader(vertex, NULL, 0);
		pContext->PSSetShader(pixel, NULL, 0);
		pContext->GSSetShader(NULL, NULL, 0);

		// バッファを設定
		const auto& vb = vertexBuffer_[mesh.vertexBuffer];
		pContext->IASetVertexBuffers(0, 1, &vb.buffer, &vb.stride, &vb.offset);
		pContext->IASetIndexBuffer(indexBuffer_[mesh.indexBuffer], DXGI_FORMAT_R16_UINT, 0);

		// 描画
		pContext->DrawIndexed((uint)mesh.index.size(), 0, 0);
	}
}

void Dx11Wrapper::Draw(const Particle* obj, const Shader* shader)
{
	const auto& pContext = directX11_->GetDeviceContext();
	if (!pContext) { return; }

	const auto& vertex   = vertexShader_[shader->GetVertexShader()];
	const auto& pixel    = pixelShader_[shader->GetPixelShader()].shader;
	const auto& sampler  = pixelShader_[shader->GetPixelShader()].sampler;
	const auto& constant = constantBuffer_[shader->GetConstantBuffer(0)];

	shader->SetParam(MATRIX().Identity().Create(&obj->GetTransform()), obj->GetColor(), VECTOR4(0, 0, 1, 1));

	// アルファブレンドのセット
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pContext->OMSetBlendState(blendState_[(int)ALFA_BREND::ADD], blendFactor, 0xffffff);
	// 深度ステンシルの設定
	pContext->OMSetDepthStencilState(depthState_, 1);
	pContext->IASetInputLayout(inputLayout_[2]);

	// テクスチャサンプラの設定
	pContext->PSSetSamplers(0, 1, &sampler);

	// シェーダーの設定
	pContext->VSSetShader(vertex, NULL, 0);
	pContext->PSSetShader(pixel, NULL, 0);
	pContext->GSSetShader(geometryShader_[0], NULL, 0);

	// バッファを設定
	const auto& vb = vertexBuffer_[obj->GetVertexBuffer()];
	pContext->IASetVertexBuffers(0, 1, &vb.buffer, &vb.stride, &vb.offset);
	pContext->VSSetConstantBuffers(0, 1, &constant);	// cbufferを使うVSに設定
	pContext->GSSetConstantBuffers(0, 1, &constant);

	// テクスチャの設定
	SetTexture(0, (int)obj->GetTexNum());

	// プリミティブトポロジーを設定
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pContext->Draw(1, 0);

	pContext->OMSetDepthStencilState(NULL, 1);
}

void Dx11Wrapper::Draw(const ColliderRenderer* obj)
{
	const auto& dev = directX11_->GetDeviceContext();
	if (!dev) { return; }

	const auto& constant = constantBuffer_[shader_[1].constantBuffer[1]];
	const auto& vertex   = vertexShader_[shader_[1].vertexShader[1]];
	const auto& pixel	 = pixelShader_[shader_[1].pixelShader[0]].shader;

	MATRIX mtx = mtx.Identity();

	const auto& transMtx = obj->GetTransMtx();
	VECTOR3 mScale = VECTOR3(1);
	if (transMtx)
	{
		mScale = VECTOR3(1 / transMtx->_11, 1 / transMtx->_22, 1 / transMtx->_33);
	}
	const auto& scale = obj->GetTransform().scale;
	VECTOR3 s = VECTOR3(1 / scale.x, 1 / scale.y, 1 / scale.z);
	mtx.Scaling(obj->GetSize() * s * mScale);
	mtx.Rotation(obj->GetOffsetRotation() * s * mScale);
	mtx.Translation(obj->GetOffsetPosition() * s * mScale);

	const auto& parentMtx = obj->GetParentMtx();
	if (parentMtx)
	{
		MATRIX temp = *parentMtx;
		if (transMtx)
		{
			temp *= *transMtx;
		}
		temp.Scaling(scale);
		mtx *= temp;
	}

	auto tr = obj->GetTransform();

	for (;;)
	{
		mtx.Scaling(tr.scale);
		mtx.Rotation(tr.rotation);
		mtx.Translation(tr.position);

		if (tr.parentMtx)
		{
			MATRIX temp = *tr.parentMtx;
			mtx *= temp;
		}
		if (tr.parent)
		{
			tr = *tr.parent;
		}
		else
		{
			break;
		}
	}

	// コンスタントバッファの設定
	SHADER_DEFAULT_OBJECT sg;
	sg.world	= mtx;
	sg.texcoord = VECTOR4(0, 0, 1, 1);
	sg.diffuse  = obj->GetColor();
	dev->UpdateSubresource(constant, 0, NULL, &sg, 0, 0);
	dev->VSSetConstantBuffers(0, 1, &constantBuffer_[shader_[1].constantBuffer[0]]);
	dev->VSSetConstantBuffers(1, 1, &constant);	// cbufferを使うVSに設定

	SetTexture(0, (int)Resources::Texture::Base::WHITE);
	dev->VSSetShader(vertex, NULL, 0);
	dev->PSSetShader(pixel, NULL, 0);
	dev->GSSetShader(NULL, NULL, 0);

	// バッファを設定
	const auto& vb = vertexBuffer_[obj->GetVertexBuffer()];
	dev->IASetVertexBuffers(0, 1, &vb.buffer, &vb.stride, &vb.offset);

	// プリミティブトポロジーを設定
	dev->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	// 描画
	dev->Draw(obj->GetVertexNum(), 0);
}

void Dx11Wrapper::BeginDrawCanvasRenderer(void)
{
	const auto& pContext = directX11_->GetDeviceContext();
	if (!pContext) { return; }

	if(const auto& window = directX11_->GetWindow())
	{
		if (const auto& systems = window->GetSystems())
		{
			if (const auto& scene = systems->GetSceneManager())
			{
				if (const auto& camera = scene->GetCameraManager())
				{
					inverse_.Billboard(camera->GetView());
				}
			}
		}
	}

	//頂点インプットレイアウトをセット
	pContext->IASetInputLayout(inputLayout_[0]);

	pContext->RSSetState(rasterizerState_);

	// アルファブレンドのセット
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pContext->OMSetBlendState(blendState_[(int)ALFA_BREND::DEF], blendFactor, 0xffffffff);

	// 深度ステンシルの設定
	pContext->OMSetDepthStencilState(depthState_, 1);

	// スクリーン座標の設定
	const auto& constant = constantBuffer_[shader_[0].constantBuffer[0]];
	VECTOR4 screen = VECTOR4(Windows::WIDTH, Windows::HEIGHT, 0, 0);
	pContext->UpdateSubresource(constant, 0, NULL, &screen, 0, 0);
	pContext->VSSetConstantBuffers(0, 1, &constant);
	pContext->GSSetShader(NULL, NULL, 0);
}

void Dx11Wrapper::BeginDrawObjectRenderer(void)
{
	const auto& pContext = directX11_->GetDeviceContext();
	if (!pContext) { return; }

	//頂点インプットレイアウトをセット
	pContext->IASetInputLayout(inputLayout_[1]);
	pContext->RSSetState(rasterizerState_);
	// アルファブレンドのセット
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pContext->OMSetBlendState(blendState_[(int)ALFA_BREND::DEF], blendFactor, 0xffffffff);
	pContext->OMSetDepthStencilState(NULL, 1);

	SHADER_DEFAULT_SCENE sg;
	const auto& cameraManager = directX11_->GetWindow()->GetSystems()->GetSceneManager()->GetCameraManager();
	sg.view = cameraManager->GetView();
	sg.proj = cameraManager->GetProj();

	const auto& constant = constantBuffer_[shader_[1].constantBuffer[0]];
	pContext->VSSetConstantBuffers(0, 1, &constant);
	pContext->UpdateSubresource(constant, 0, NULL, &sg, 0, 0);
}

void Dx11Wrapper::EndDrawRenderer(void)
{
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }

	context->VSSetShader(nullptr, nullptr, 0);
	context->HSSetShader(nullptr, nullptr, 0);
	context->DSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

D3D11_PRIMITIVE_TOPOLOGY Dx11Wrapper::SelectPrimitiveType(PRIMITIVE::TYPE type)
{
	switch (type)
	{
	case Wrapper::PRIMITIVE::TYPE::POINT:
		return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	case Wrapper::PRIMITIVE::TYPE::LINE:
		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case Wrapper::PRIMITIVE::TYPE::LINELIST:
		return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case Wrapper::PRIMITIVE::TYPE::TRIANGLE:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	case Wrapper::PRIMITIVE::TYPE::TRIANGLE_STRIP:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	case Wrapper::PRIMITIVE::TYPE::TRIANGLE_FAN:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	}
	return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
}

HRESULT Dx11Wrapper::LoadTexture(string fileName, int texNum, int modelNum)
{
	if (modelNum < 0)
	{
		UNREFERENCED_PARAMETER(texNum);
		if (texture_.size() == 0) { texture_.resize(1); }
		Dx11Texture temp;

		// ファイルを開いて格納
		std::wstring name(fileName.begin(), fileName.end());
		ID3D11Resource* descOriginal;
		HRESULT hr = DirectX::CreateWICTextureFromFile(directX11_->GetDevice(), name.c_str(), &descOriginal, &temp.data);
		string na = fileName;
		string no = "が開けませんでした";
		na += no;
		if (directX11_->GetWindow()->ErrorMessage(na.c_str(), "エラー", hr)) { return E_FAIL; }
		// テクスチャサイズの取得
		D3D11_TEXTURE2D_DESC desc;
		static_cast<ID3D11Texture2D*>(descOriginal)->GetDesc(&desc);
		ReleasePtr(descOriginal);

		temp.size.x = (float)desc.Width;
		temp.size.y = (float)desc.Height;

		texture_[0].emplace_back(temp);
	}
	else
	{
		if (texture_.size() < (uint)modelNum + 2)
		{
			std::vector<Dx11Texture> t;
			texture_.emplace_back(t);
		}
		Dx11Texture temp;

		// ファイルを開いて格納
		std::wstring name(fileName.begin(), fileName.end());
		ID3D11Resource* descOriginal;
		HRESULT hr = DirectX::CreateWICTextureFromFile(directX11_->GetDevice(), name.c_str(), &descOriginal, &temp.data);
		string na = fileName;
		string no = "が開けませんでした";
		na += no;
		if (directX11_->GetWindow()->ErrorMessage(na.c_str(), "エラー", hr)) { return E_FAIL; }
		// テクスチャサイズの取得
		D3D11_TEXTURE2D_DESC desc;
		static_cast<ID3D11Texture2D*>(descOriginal)->GetDesc(&desc);
		ReleasePtr(descOriginal);

		temp.size.x = (float)desc.Width;
		temp.size.y = (float)desc.Height;

		texture_[modelNum + 1].emplace_back(temp);
	}

	return S_OK;
}

void Dx11Wrapper::ReleaseTexture(int texNum, int modelNum)
{
	for (int i = 0; i < 8; ++i) { SetTexture(i, -1); }

	if (static_cast<uint>(modelNum + 1) >= texture_.size()) { return; }
	auto& tex = texture_[modelNum + 1];

	if (modelNum >= 0)
	{
		int texSize = static_cast<int>(tex.size());
		for (int i = texSize - 1; i >= 0; --i)
		{
			ReleasePtr(tex[i].data);

			auto& thi = tex[i];
			for (auto itr = tex.begin(); itr != tex.end();)
			{
				if (&(*itr) == &thi)
				{
					itr = tex.erase(itr);		//配列削除
					break;
				}
				else { itr++; }
			}
			tex.shrink_to_fit();
		}
		for (auto itr = texture_.begin(); itr != texture_.end();)
		{
			if (&(*itr) == &tex)
			{
				itr = texture_.erase(itr);		//配列削除
				break;
			}
			else { itr++; }
		}
		tex.shrink_to_fit();
	}
	else
	{
		if (tex.size() <= static_cast<uint>(texNum)) { return; }
		ReleasePtr(tex[texNum].data);

		auto& thi = tex[texNum];
		for (auto itr = tex.begin(); itr != tex.end();)
		{
			if (&(*itr) == &thi)
			{
				itr = tex.erase(itr);		//配列削除
				break;
			}
			else { itr++; }
		}
		tex.shrink_to_fit();
	}
}

HRESULT Dx11Wrapper::LoadModel(string fileName, int modelNum)
{
	LoadM Loader;
	MODEL tempModel;

	string extension;
	for (int i = (int)fileName.size() - 1; i > 0 && fileName[i] != '.'; --i)
	{		
		extension.insert(extension.begin(), fileName[i]);
	}

	if (extension == "bmx")
	{
		tempModel = Loader.Load(fileName);
	}

	for (auto& b : tempModel.bone)
	{
		XMVECTOR t;
		XMMATRIX temp = XMMatrixInverse(&t, XM(b.offsetMtx));
		b.inverseMtx = V(temp);
	}

	int texNum = 0;
	for (auto& mesh : tempModel.mesh)
	{
		mesh.computeShader = R_ERROR;
		mesh.vertexBuffer = CreateVertexBuffer(&mesh.vertex[0], sizeof(mesh.vertex[0]), (uint)mesh.vertex.size());
		if (mesh.vertexBuffer == R_ERROR) { return E_FAIL; }

		mesh.indexBuffer = CreateIndexBuffer(&mesh.index[0], (uint)mesh.index.size());
		if (mesh.indexBuffer == R_ERROR) { return E_FAIL; }

		int texMax = static_cast<int>(MaterialType::MAX);
		for (int j = 0;j < texMax;++j)
		{
			mesh.material.texture[j] = 0;
			string tempName = mesh.material.textureName[j];
			if (tempName != "")
			{
				string directory = fileName;
				// テクスチャのディレクトリはモデルと同じ
				for (uint i = (uint)directory.size() - 1; directory[i] != '/' && i > 0; i--) { directory.pop_back(); }

				string texName;
				int size = (int)tempName.size() - 1;
				for (int i = size; i >= 0; --i)
				{
					if (tempName[i] == 92 || tempName[i] == '/') { break; }
					texName.insert(texName.begin(), tempName[i]);
				}
				mesh.material.textureName[j] = tempName.c_str();
				directory += texName;

				if (SUCCEEDED(LoadTexture(directory, texNum, modelNum)))
				{
					mesh.material.texture[j] = texNum;
					texNum++;
				}
			}
			else
			{
				if (j == 0)
				{
					const string& temp = Systems::Instance()->GetResource().GetWhiteTextureName();
					LoadTexture(temp, texNum, modelNum);
					mesh.material.texture[j] = texNum;
					texNum++;
				}
			}
		}
	}

	if (texNum > 0)
	{
		model_.emplace_back(tempModel);
	}

	return S_OK; 
}

HRESULT Dx11Wrapper::LoadModelAnimation(string fileName, int parent)
{
	LoadM	Loader;
	MODEL	tempModel;

	if (parent < 0) { return E_FAIL; }

	string extension;
	for (int i = (int)fileName.size() - 1; i > 0 && fileName[i] != '.'; --i)
	{
		extension.insert(extension.begin(), fileName[i]);
	}

	if (extension == "bamx")
	{
		HRESULT hr = Loader.LoadAnimation(fileName, model_[parent]);

		string error = fileName + "が開けませんでした";

		if (directX11_->GetWindow()->ErrorMessage(error.c_str(), "エラー", hr)) { return E_FAIL; }

		for (auto& cs : model_[parent].mesh)
		{
			if (cs.computeShader == R_ERROR)
			{
//				cs.computeShader = CreateComputeShader(Define::ResourceDirectoryName + "Data/Skinning.cso", "CS_Main", "cs_5_0", &cs.vertex[0], sizeof(VERTEX), cs.vertex.size());
//				if (cs.computeShader == R_ERROR) { return E_FAIL; }
			}
		}
		return S_OK;
	}

	return E_FAIL;
}

void Dx11Wrapper::ReleaseModel(int modelNum)
{
	int size = static_cast<int>(model_.size());
	if (size <= modelNum) { return; }

	for (auto& mesh : model_[modelNum].mesh)
	{
		mesh.material.texture[0] = 0;
		mesh.material.texture[1] = 0;
		mesh.material.textureName[0] = "";
		mesh.material.textureName[1] = "";
		ReleaseBuffer(mesh.vertexBuffer, Wrapper::FVF::VERTEX_3D);
		ReleaseBuffer(mesh.indexBuffer, Wrapper::FVF::INDEX);
	}
	ReleaseTexture(0, modelNum);

	auto& thi = model_[modelNum];

	for (auto itr = model_.begin(); itr != model_.end();)
	{
		if (&(*itr) == &thi)
		{
			itr = model_.erase(itr);		//配列削除
			break;
		}
		else
		{
			itr++;
		}
	}
	model_.shrink_to_fit();

}

ID3DBlob* Dx11Wrapper::CompiledShader(string fileName, string method, string version)
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob* pCompiledShader	= nullptr;
	ID3DBlob* pErrors			= nullptr;

	setlocale(LC_ALL, "japanese");
	size_t  len = 0;
	WCHAR	wStr[256];
	mbstowcs_s(&len, wStr, fileName.size() + 1, fileName.c_str(), _TRUNCATE);
	//ブロブからバーテックスシェーダー作成
	HRESULT hr = D3DCompileFromFile(wStr, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, method.c_str(), version.c_str(), D3DCOMPILE_ENABLE_STRICTNESS, 0, &pCompiledShader, &pErrors);
	if (directX11_->GetWindow()->ErrorMessage((fileName + method).c_str(), "エラー", hr))
	{
		OutputDebugStringA((const char*)pErrors->GetBufferPointer());

		__debugbreak();
		return nullptr;
	}
	ReleasePtr(pErrors);

	return pCompiledShader;
}

long Dx11Wrapper::ReadShader(string csoName, byte** b)
{
	FILE* fp;
	int ret = fopen_s(&fp, csoName.c_str(), "rb");
	if (ret != 0) { return -1; }
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	*b = new byte[size];

	fread(*b, size, 1, fp);
	fclose(fp);
	return size;
}

uint Dx11Wrapper::CreateVertexShader(string fileName, string method, string version)
{
	if (version != "vs_5_0") { return 0; }

	const auto& dev = directX11_->GetDevice();
	HRESULT hr;
	ID3D11VertexShader* tempVertexShader;

	ID3DBlob* pCompiledShader = CompiledShader(fileName, method, version);
	if (!pCompiledShader) { return R_ERROR; }

	hr = dev->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &tempVertexShader);
	if (directX11_->GetWindow()->ErrorMessage(string(fileName + "バーテックスシェーダー作成失敗").c_str(), "エラー", hr))
	{
		__debugbreak();
		ReleasePtr(pCompiledShader);
		return R_ERROR;
	}
	ReleasePtr(pCompiledShader);
	vertexShader_.emplace_back(tempVertexShader);
	return (uint)vertexShader_.size() - 1;
}

uint Dx11Wrapper::CreatePixelShader(string fileName, string method, string version)
{
	if (version != "ps_5_0") { return 0; }

	const auto& pDevice = directX11_->GetDevice();
	HRESULT hr;
	PixelShader tempPixelShader;

	ID3DBlob* pCompiledShader = CompiledShader(fileName, method, version);
	if (!pCompiledShader) { return R_ERROR; }

	hr = pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &tempPixelShader.shader);
	if (directX11_->GetWindow()->ErrorMessage("ピクセルシェーダーの作成に失敗", "エラー", hr))
	{
		ReleasePtr(pCompiledShader);
		return R_ERROR;
	}
	ReleasePtr(pCompiledShader);

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	hr = pDevice->CreateSamplerState(&SamDesc, &tempPixelShader.sampler);
	if (directX11_->GetWindow()->ErrorMessage("テクスチャサンプラの作成に失敗", "エラー", hr)) { return R_ERROR; }

	pixelShader_.emplace_back(tempPixelShader);

	return (uint)pixelShader_.size() - 1;
}

uint Dx11Wrapper::CreateGeometryShader(string fileName, string method, string version)
{
	const auto& pDevice = directX11_->GetDevice();
	HRESULT hr;
	ID3D11GeometryShader* tempGeometryShader;

	ID3DBlob* pCompiledShader = CompiledShader(fileName, method, version);
	if (!pCompiledShader) { return R_ERROR; }

	hr = pDevice->CreateGeometryShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &tempGeometryShader);
	if (directX11_->GetWindow()->ErrorMessage("ジオメトリシェーダーの作成に失敗", "エラー", hr))
	{
		ReleasePtr(pCompiledShader);
		return R_ERROR;
	}
	ReleasePtr(pCompiledShader);

	geometryShader_.emplace_back(tempGeometryShader);
	return (uint)geometryShader_.size() - 1;
}

uint Dx11Wrapper::CreateComputeShader(string fileName, string method, string version, const void* v, uint size, uint num)
{
	const auto& dev = directX11_->GetDevice();
	HRESULT hr;
	ComputeShader tempComputeShader;

	string extension;
	for (int i = (int)fileName.size() - 1; i > 0 && fileName[i] != '.'; --i)
	{
		extension.insert(extension.begin(), fileName[i]);
	}

	ID3DBlob* pCompiledShader = nullptr;
	byte* b = nullptr,*temp = nullptr;
	long  s    = -1;
	if (extension == "cso")
	{
		s = ReadShader(fileName, &temp);
		b = temp;
	}
	else
	{
		pCompiledShader = CompiledShader(fileName, method, version);
		if (!pCompiledShader) { return R_ERROR; }

		b = (byte*)pCompiledShader->GetBufferPointer();
		s	 = (long)pCompiledShader->GetBufferSize();
	}

	hr = dev->CreateComputeShader(b, s, NULL, &tempComputeShader.shader);
	if (directX11_->GetWindow()->ErrorMessage("コンピュートシェーダーの作成に失敗", "エラー", hr))
	{
		ReleasePtr(pCompiledShader);
		if (temp) { delete[] temp; }
		return R_ERROR;
	}
	ReleasePtr(pCompiledShader);
	if (temp) { delete[] temp; }

	D3D11_BUFFER_DESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(bufDesc));
	bufDesc.ByteWidth			= size * num;
	bufDesc.StructureByteStride = size;
	bufDesc.Usage				= D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags			= D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem				= v;
	subData.SysMemPitch			= 0;
	subData.SysMemSlicePitch	= 0;
	hr = dev->CreateBuffer(&bufDesc, NULL, &tempComputeShader.buffer);
	if (FAILED(hr)) { return R_ERROR; }

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement	= 0;
	srvDesc.BufferEx.NumElements	= bufDesc.ByteWidth / bufDesc.StructureByteStride;
	srvDesc.BufferEx.Flags			= 0;
	hr = dev->CreateShaderResourceView(tempComputeShader.buffer, &srvDesc, &tempComputeShader.shaderResource);
	if (FAILED(hr)) { return R_ERROR; }

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format				= DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension		= D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements	= bufDesc.ByteWidth / bufDesc.StructureByteStride;
	uavDesc.Buffer.Flags		= 0;
	hr = dev->CreateUnorderedAccessView(tempComputeShader.buffer, &uavDesc, &tempComputeShader.unorderedAcces);
	if (FAILED(hr)) { return R_ERROR; }

	computeShader_.emplace_back(tempComputeShader);
	return (uint)computeShader_.size() - 1;
}

uint Dx11Wrapper::CreateConstantBuffer(uint size)
{
	const auto& pDevice = directX11_->GetDevice();

	D3D11_BUFFER_DESC cb;
	cb.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth			= size;		// ※16の倍数でなければならない
	cb.Usage				= D3D11_USAGE_DEFAULT;
	cb.CPUAccessFlags		= 0;
	cb.MiscFlags			= 0;
	cb.StructureByteStride	= 0;

	ID3D11Buffer* temp;
	HRESULT hr = pDevice->CreateBuffer(&cb, NULL, &temp);
	if (directX11_->GetWindow()->ErrorMessage("コンスタントバッファの作成に失敗", "エラー", hr)) { return R_ERROR; }

	constantBuffer_.emplace_back(temp);
	return (uint)constantBuffer_.size() - 1;
}

void Dx11Wrapper::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* elem, int size, string fileName)
{
	const auto& dev = directX11_->GetDevice();
	HRESULT hr;
	ID3DBlob* pCompiledShader = CompiledShader(fileName, "VS_Main", "vs_5_0");
	if (!pCompiledShader) { return; }

	ID3D11InputLayout* tempInputLayout;
	hr = dev->CreateInputLayout(elem, size, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &tempInputLayout);
	if (directX11_->GetWindow()->ErrorMessage(string(fileName + "頂点インプットレイアウトの作成に失敗").c_str(), "エラー", hr)) 
	{
		__debugbreak();
		ReleasePtr(pCompiledShader);
		return;
	}
	ReleasePtr(pCompiledShader);

	inputLayout_.emplace_back(tempInputLayout);
}

void Dx11Wrapper::DrawQuad(VECTOR2 position, VECTOR2 size, COLOR color)
{
	BeginDrawCanvasRenderer();

	const auto& pContext = directX11_->GetDeviceContext();
	if (!pContext) { return; }

	int pixelNum = 0;
	const auto& vertex = vertexShader_[shader_[0].vertexShader[0]];
	const auto& pixel = pixelShader_[shader_[0].pixelShader[pixelNum]].shader;
	const auto& sampler = pixelShader_[shader_[0].pixelShader[pixelNum]].sampler;

	// テクスチャサンプラの設定
	pContext->PSSetSamplers(0, 1, &sampler);

	const auto& constant = constantBuffer_[shader_[0].constantBuffer[1]];
	SHADER_UI c;
	c.position = VECTOR4(position.x, position.y, 1, 1);
	c.sizeSplit.x = size.x;
	c.sizeSplit.y = size.y;
	c.color		  = color;

	pContext->UpdateSubresource(constant, 0, NULL, &c, 0, 0);
	pContext->VSSetConstantBuffers(1, 1, &constant);

	//使用するシェーダーの設定
	pContext->VSSetShader(vertex, NULL, 0);
	pContext->PSSetShader(pixel, NULL, 0);

	// 頂点バッファをセット
	const auto& vb = vertexBuffer_[0];
	pContext->IASetVertexBuffers(0, 1, &vb.buffer, &vb.stride, &vb.offset);

	// プリミティブトポロジーを設定
	pContext->IASetPrimitiveTopology(SelectPrimitiveType(Wrapper::PRIMITIVE::TYPE::TRIANGLE_STRIP));
	// 描画
	pContext->Draw(2 * 2, 0);

	EndDrawRenderer();
}

void Dx11Wrapper::SetShaderValue(const int buffer, const int valueNum, const string* nameArray, const int* valueSizeArray, const void* valueArray)
{
	UNREFERENCED_PARAMETER(valueNum);
	UNREFERENCED_PARAMETER(nameArray);
	UNREFERENCED_PARAMETER(valueSizeArray);

	const auto& context = directX11_->GetDeviceContext();
	const auto& constant = constantBuffer_[buffer];
	context->UpdateSubresource(constant, 0, NULL, valueArray, 0, 0);

	/* OpenGL用メモ
	const char* value = (char*)valueArray;
	int bit = 0;
	for (int i = 0; i < valueNum; ++i)
	{
		int size = valueSizeArray[i];
		char* temp = new char[size];
		memcpy_s(temp, size, &value[bit], size);
		if (size == 64)
		{
			MATRIX* check = (MATRIX*)temp;
			MATRIX checkValue;
			for (int j = 0; j < 16; ++j) { checkValue.m[j / 4][j % 4] = check->m[j / 4][j % 4]; }
		}
		bit += size;
	}*/
}

void Dx11Wrapper::SetConstantBuffer(ShaderType type, int startSrot, int numBuffers, int constantBuffer)
{
	const auto& context = directX11_->GetDeviceContext();

	if (type == ShaderType::Vertex)
	{
		context->VSSetConstantBuffers(startSrot, numBuffers, &constantBuffer_[constantBuffer]);
	}
	else if (type == ShaderType::Pixel)
	{
		context->PSSetConstantBuffers(startSrot, numBuffers, &constantBuffer_[constantBuffer]);
	}
}
