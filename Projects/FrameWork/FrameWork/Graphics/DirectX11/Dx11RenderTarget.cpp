#include "Dx11RenderTarget.h"
#include "DirectX11.h"
#include "../../Windows/Windows.h"
#include "../../Systems/Renderer/Sprite/Texture.h"

#include "../../Systems/GameSystems.h"
#include "../../Systems/Renderer/Shader/ZTexture.h"
#include "../../Scene/SceneManager.h"
#include "../../Systems/Light.h"
#include "../../../../Sources/Scene/GameScene.h"
#include "../../Systems/Camera/CameraManager.h"

/* @biref	コンストラクタ
 * @param	(dx11)	親のポインタ		*/
Dx11RenderTarget::Dx11RenderTarget(DirectX11* dx11) :
	directX11_(dx11)
	, shadowSampler_(nullptr)
{
	for (auto& r : renderTargetView_)	{ r = nullptr; }
	for (auto& s : shaderResourceView_) { s = nullptr; }

	cascadeConfig_.cascadeLevels = MAX_CASCADE;
	cascadeConfig_.bufferSize	 = 1024;
}

/* @brief	デストラクタ				*/
Dx11RenderTarget::~Dx11RenderTarget(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	成功失敗					*/
HRESULT Dx11RenderTarget::Init(void)
{
	if (!directX11_)	{ return E_FAIL; }
	const auto& window = directX11_->GetWindow();
	if (!window)		{ return E_FAIL; }

	const auto& swapChain = directX11_->GetSwapChain();
	if (!swapChain) { return E_FAIL; }

	// バックバッファの情報を取得
	renderTargetView_[static_cast<int>(List::DEFAULT)] = directX11_->GetRenderTargetView();
	depthStencilView_ = directX11_->GetDepthStencilView();

	// スクリーンショット用にコピー
	renderTargetView_[static_cast<int>(List::SCREEN_SHOT)] = directX11_->GetRenderTargetView();

	HRESULT hr;
	hr = CreateNormalRenderTarget(List::COLOR);
	if (window->ErrorMessage("カラーの初期化に失敗しました。"		 , "エラー", hr)) { return hr; }
	hr = CreateNormalRenderTarget(List::POSITION);					 
	if (window->ErrorMessage("ポジションの初期化に失敗しました。"	 , "エラー", hr)) { return hr; }
	hr = CreateNormalRenderTarget(List::NORMAL);					 
	if (window->ErrorMessage("ノーマルの初期化に失敗しました。"		 , "エラー", hr)) { return hr; }
	hr = CreateShadowmapRenderTarget();
	if (window->ErrorMessage("シャドウマップの初期化に失敗しました。", "エラー", hr)) { return hr; }

	return S_OK;
}

/* @brief	後処理
 * @param	なし
 * @return	なし						*/
void Dx11RenderTarget::Uninit(void)
{
	ReleasePtr(shadowSampler_);
	ReleasePtr(depthStencilView_);

	for (auto& r : renderTargetView_)	{ ReleasePtr(r); }
	for (auto& s : shaderResourceView_) { ReleasePtr(s); }

}

/* @brief	普通のレンダーターゲットの作成
 * @param	(num)	レンダーターゲットの種類
 * @return	成功失敗					*/
HRESULT Dx11RenderTarget::CreateNormalRenderTarget(List num)
{
	if (!directX11_) { return E_FAIL; }
	const auto& window = directX11_->GetWindow();
	if (!window) { return E_FAIL; }

	const auto& swapChain = directX11_->GetSwapChain();
	const auto& device = directX11_->GetDevice();

	if (!swapChain) { return E_FAIL; }
	if (!device) { return E_FAIL; }

	ID3D11Texture2D* temp;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&temp);

	D3D11_TEXTURE2D_DESC td;
	temp->GetDesc(&td);
	td.SampleDesc.Count		= 1;
	td.SampleDesc.Quality	= 0;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D* tex2D;
	HRESULT hr = device->CreateTexture2D(&td, nullptr, &tex2D);
	if (window->ErrorMessage("テクスチャ2Dの作成に失敗しました。", "エラー", hr)) { return hr; }

	if (FAILED(CreateRenderTarget(num, tex2D, nullptr, nullptr))) { return E_FAIL; }

	ReleasePtr(tex2D);

	return S_OK;
}

/* @brief	シャドウマップのレンダーターゲットの作成
 * @param	なし
 * @return	成功失敗					*/
HRESULT Dx11RenderTarget::CreateShadowmapRenderTarget(void)
{
	if (!directX11_) { return E_FAIL; }
	const auto& window = directX11_->GetWindow();
	if (!window) { return E_FAIL; }

	const auto& swapChain = directX11_->GetSwapChain();
	const auto& device = directX11_->GetDevice();

	if (!swapChain) { return E_FAIL; }
	if (!device) { return E_FAIL; }

	ID3D11Texture2D* tex2D;
	// 深度テクスチャの生成.
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width				= Graphics::WIDTH  * 4;
	texDesc.Height				= Graphics::HEIGHT * 4;
	texDesc.Format				= DXGI_FORMAT_R32_TYPELESS;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality  = 0;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MiscFlags			= 0;

	HRESULT hr =  device->CreateTexture2D(&texDesc, nullptr, &tex2D);
	if (FAILED(hr)) { return hr; }

	// 深度ステンシルビューではαが使えず、ビルボードの影が作れない
	// レンダーターゲットビューの生成.
	D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
	ZeroMemory(&rtDesc, sizeof(rtDesc));
	rtDesc.Format				= DXGI_FORMAT_R32_FLOAT;
	rtDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
	rtDesc.Texture2D.MipSlice = 0;

	// シャドウマップシェーダリソースビューの生成.
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	ZeroMemory(&srDesc, sizeof(srDesc));
	srDesc.Format						= DXGI_FORMAT_R32_FLOAT;
	srDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MipLevels			= 1;
	srDesc.Texture2D.MostDetailedMip	= 0;

	if (FAILED(CreateRenderTarget(List::SHADOW, tex2D, &rtDesc, &srDesc))) { return E_FAIL; }

	ReleasePtr(tex2D);

	// 専用サンプラーを作る
	// これでやらないと描画が重い
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.AddressU			= D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV			= D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW			= D3D11_TEXTURE_ADDRESS_BORDER;
	sd.BorderColor[0]	= 1.0f;
	sd.BorderColor[1]	= 1.0f;
	sd.BorderColor[2]	= 1.0f;
	sd.BorderColor[3]	= 1.0f;
	sd.ComparisonFunc	= D3D11_COMPARISON_LESS_EQUAL;
	sd.Filter			= D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy	= 1;
	sd.MipLODBias		= 0;
	sd.MinLOD			= -FLT_MAX;
	sd.MaxLOD			= +FLT_MAX;

	// サンプラーステートを生成.
	hr = device->CreateSamplerState(&sd, &shadowSampler_);

	// シャドウマップ用ビューポート
	shadowViewport_.Width	 = (float)texDesc.Width;
	shadowViewport_.Height	 = (float)texDesc.Height;
	shadowViewport_.MinDepth = 0.0f;
	shadowViewport_.MaxDepth = 1.0f;
	shadowViewport_.TopLeftX = 0;
	shadowViewport_.TopLeftY = 0;

	return S_OK;
}

/* @brief	レンダーターゲットの作成
 * @param	(num)	レンダーターゲットの種類
 * @param	(tex2D)	テクスチャリソース
 * @return	成功失敗					*/
HRESULT Dx11RenderTarget::CreateRenderTarget(List num, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srDesc)
{
	if (!directX11_)	{ return E_FAIL; }
	const auto& window = directX11_->GetWindow();
	if (!window)		{ return E_FAIL; }
	const auto& device = directX11_->GetDevice();
	if (!device)	{ return E_FAIL; }

	int listNum = static_cast<int>(num);
	HRESULT hr;
	hr = device->CreateRenderTargetView(tex2D, (rtDesc) ? rtDesc : 0, &renderTargetView_[listNum]);
	if (window->ErrorMessage("レンダーターゲットビューの作成に失敗しました。", "エラー", hr)) { return hr; }

	hr = device->CreateShaderResourceView(tex2D, (srDesc) ? srDesc : 0, &shaderResourceView_[listNum]);
	if (window->ErrorMessage("シェーダーリソースビューの作成に失敗しました。", "エラー", hr)) { return hr; }

	return S_OK;
}

/* @brief	レンダーターゲットのクリア
 * @param	(num)	レンダーターゲットの種類
 * @return	なし						*/
void Dx11RenderTarget::ClearRendererTarget(List num, COLOR color)
{
	if (!directX11_) { return; }
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }

	int listNum = static_cast<int>(num);

	context->ClearRenderTargetView(renderTargetView_[listNum], (float*)color);
	context->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

/* @brief	マルチレンダーターゲットの開始
 * @param	なし
 * @return	なし						*/
void Dx11RenderTarget::BeginMultiRendererTarget(void)
{
	if (!directX11_)	{ return; }
	const auto& context = directX11_->GetDeviceContext();
	if (!context)		{ return; }

	ID3D11RenderTargetView* nullRTV = nullptr; 
	context->OMSetRenderTargets(1, &nullRTV, 0);
	context->OMSetRenderTargets(3, &renderTargetView_[static_cast<int>(List::COLOR)], depthStencilView_);
	COLOR color[3] = { COLOR(0.5f, 0.2f, 0.3f, 1), COLOR(0.2f, 0.5f, 0.3f, 1), COLOR(0.3f, 0.2f, 0.5f, 1) };
	for (int i = 0; i < 3; ++i)
	{
		int n = static_cast<int>(List::COLOR) + i;
		ClearRendererTarget(static_cast<List>(n), color[i]);
	}
}

/* @brief	マルチレンダーターゲットの終了
 * @param	なし
 * @return	なし						*/
void Dx11RenderTarget::EndMultiRendererTarget(void)
{
	if (!directX11_) { return; }
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }
	const auto& wrapper = directX11_->GetWrapper();
	if (!wrapper) { return; }

	ID3D11RenderTargetView* nullRTV = nullptr;
	context->OMSetRenderTargets(1, &nullRTV, 0);
	context->OMSetRenderTargets(1, &renderTargetView_[static_cast<int>(List::DEFAULT)], depthStencilView_);
	ClearRendererTarget(List::DEFAULT, COLOR::RGBA(32, 100, 92, 255));

	int draw = static_cast<int>((debugDraw_ == List::MAX) ? List::COLOR : debugDraw_);
	context->PSSetShaderResources(0, 1, &shaderResourceView_[draw]);
	wrapper->DrawQuad(VECTOR2((float)Half(Graphics::WIDTH), (float)Half(Graphics::HEIGHT)), VECTOR2((float)Windows::WIDTH, (float)Windows::HEIGHT));
}

/* @brief	レンダーターゲットの描画
 * @param	(num)	レンダーターゲットの種類
 * @return	なし						*/
void Dx11RenderTarget::Draw(List num, VECTOR2 position, VECTOR2 size)
{
	if (!directX11_) { return; }
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }
	const auto& wrapper = directX11_->GetWrapper();
	if (!wrapper) { return; }

	int listNum = static_cast<int>(num);
	context->PSSetShaderResources(0, 1, &shaderResourceView_[listNum]);
	wrapper->DrawQuad(position, size);
	ID3D11ShaderResourceView* temp = nullptr;
	context->PSSetShaderResources(0, 1, &temp);
}

/* @brief	スクリーンショットの作成
 * @param	(filename)	ファイル名
 * @return	なし						*/
void Dx11RenderTarget::CreateScreenshot(const string& filename)
{
	int n = static_cast<int>(List::SCREEN_SHOT);
	if (shaderResourceView_[n]) { ReleasePtr(shaderResourceView_[n]); }

	const auto& dev = directX11_->GetDevice();

	ID3D11Resource* buf;
	renderTargetView_[n]->GetResource(&buf);
	D3D11_RESOURCE_DIMENSION type;
	buf->GetType(&type);
	if (type != D3D11_RESOURCE_DIMENSION_TEXTURE2D) { return; }

	D3D11_TEXTURE2D_DESC td;
	static_cast<ID3D11Texture2D*>(buf)->GetDesc(&td);
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	ID3D11Texture2D* tempTexture;
	dev->CreateTexture2D(&td, NULL, &tempTexture);

	const auto& pContext = directX11_->GetDeviceContext();
	// マルチサンプリングをしている場合
	pContext->ResolveSubresource(tempTexture, 0, buf, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
	// マルチサンプリングを使用していない場合
	//	pContext->CopyResource(tempTexture, screenShot_.pBuffer);
	const auto temp = D3DX11SaveTextureToFile(pContext, tempTexture, D3DX11_IFF_BMP, filename.c_str());

	// 吐き出したテクスチャを読み込み
	std::wstring name(filename.begin(), filename.end());
	ID3D11Resource* descOriginal;
	DirectX::CreateWICTextureFromFile(directX11_->GetDevice(), name.c_str(), &descOriginal, &shaderResourceView_[n]);
}

/* @brief	シャドウマップの描画開始
 * @param	なし
 * @return	なし						*/
void Dx11RenderTarget::BeginDrawShadow(void)
{
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }

	Temp();

	int n = static_cast<int>(List::SHADOW);

	ID3D11RenderTargetView * nullRTV = nullptr;
	context->OMSetRenderTargets(1, &nullRTV, 0);
	context->OMSetRenderTargets(1, &renderTargetView_[n], 0);
	// クリア処理
	context->ClearRenderTargetView(renderTargetView_[n], (float*)COLOR(0, 0, 0, 0));
	context->RSSetViewports(1, &shadowViewport_);
}

/* @brief	シャドウマップの描画終了
 * @param	なし
 * @return	なし						*/
void Dx11RenderTarget::EndDrawShadow(void)
{
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }

	ID3D11RenderTargetView * nullRTV = nullptr;
	context->OMSetRenderTargets(1, &nullRTV, 0);
	context->RSSetViewports(1, &directX11_->GetViewport());
	context->OMSetRenderTargets(1, &renderTargetView_[static_cast<int>(List::DEFAULT)], depthStencilView_);
}

void Dx11RenderTarget::Temp(void)
{
}
