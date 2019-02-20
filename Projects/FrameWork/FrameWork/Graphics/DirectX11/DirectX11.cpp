//-----------------------------------------------------------------------------
//
//	DirectX11�̏���[DirectX11.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Dx11Wrapper.h"
#include "Dx11RenderTarget.h"
#include "../../Windows/Windows.h"

// �R���X�g���N�^
DirectX11::DirectX11(void) :
	pSwapChain_(nullptr)
	, pDevice_(nullptr)
	, pDeviceContext_(nullptr)
	, pRenderTargetView_(nullptr)
{
}

// ����������
HRESULT DirectX11::Init(void)
{
	// ������
	if (window_->ErrorMessage("DirectX�̏������Ɏ��s���܂���", "�G���[", InitAll())) { return E_FAIL; }

	return S_OK;
}

// �S�Ă̏���������
HRESULT DirectX11::InitAll(void)
{
	// �f�o�C�X�̐ݒ�
	if (!SetDevice())	{ return E_FAIL; }

	// ���b�p�[�̐���
	wrapper_ = new Dx11Wrapper(this);
	if (wrapper_)
	{
		wrapper_->Init();
	}

	renderTarget_ = new Dx11RenderTarget(this);
	if (renderTarget_)
	{
		renderTarget_->Init();
	}

	return S_OK;
}

// �f�o�C�X�̐ݒ�
bool DirectX11::SetDevice(void)
{
	HRESULT hr;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	//�f�o�C�X�̐���
	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &pDevice_, NULL, &pDeviceContext_);
	if (FAILED(hr)) { return false; }

	//�g�p�\��MSAA���擾
	DXGI_SAMPLE_DESC MSAA;
	MSAA.Count   = 1;
	MSAA.Quality = 0;
	//for (int i = 0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; ++i) 
	//{
	//	uint quality;
	//	hr = pDevice_->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_uint, i, &quality);
	//	if(SUCCEEDED(hr))
	//	{
	//		if (0 < quality) 
	//		{
	//			MSAA.Count = i;
	//			MSAA.Quality = quality - 1;
	//		}
	//	}
	//}

	//�C���^�[�t�F�[�X�擾
	IDXGIDevice1* pDXGI = nullptr;
	hr = pDevice_->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGI);
	if (FAILED(hr)) { return false; }

	//�A�_�v�^�[�擾
	IDXGIAdapter* pAdapter = nullptr;
	hr = pDXGI->GetAdapter(&pAdapter);
	if (FAILED(hr)) { return false; }

	//�t�@�N�g���[�擾
	IDXGIFactory* pDXGIFactory = nullptr;
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);
	if (!pDXGIFactory) { return false; }

	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));
	sd.BufferCount					= 1;
	sd.BufferDesc.Width				= Windows::WIDTH;
	sd.BufferDesc.Height			= Windows::HEIGHT;
	sd.BufferDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling			= DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc					= MSAA;

	// ���t���b�V�����[�g 60fps 
	sd.BufferDesc.RefreshRate.Numerator		= 60;		// ���q
	sd.BufferDesc.RefreshRate.Denominator	= 1;		// ����
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow							= window_->GetHWND();
	sd.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.Windowed								= !Windows::FULL_SCREEN;

	hr = pDXGIFactory->CreateSwapChain(pDevice_, &sd, &pSwapChain_);
	if (FAILED(hr)) { return false; }

	ReleasePtr(pDXGIFactory);
	ReleasePtr(pAdapter);
	ReleasePtr(pDXGI);

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	ID3D11Texture2D *pBackBuffer;
	pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pDevice_->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView_);
	pBackBuffer->Release();

	ID3D11Texture2D* depthTexture = nullptr;
	D3D11_TEXTURE2D_DESC txDesc;
	ZeroMemory(&txDesc, sizeof(txDesc));
	txDesc.Width				= Windows::WIDTH;
	txDesc.Height				= Windows::HEIGHT;
	txDesc.MipLevels			= 1;
	txDesc.ArraySize			= 1;
	txDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	txDesc.SampleDesc			= MSAA;
	txDesc.Usage				= D3D11_USAGE_DEFAULT;
	txDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	txDesc.CPUAccessFlags		= 0;
	txDesc.MiscFlags			= 0;
	hr = pDevice_->CreateTexture2D(&txDesc, NULL, &depthTexture);
	if (FAILED(hr)) { return false; }

	//�X�e���V���^�[�Q�b�g�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	depthStencilViewDesc.Format			= txDesc.Format;
	depthStencilViewDesc.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Flags			= 0;
	if (FAILED(pDevice_->CreateDepthStencilView(depthTexture, &depthStencilViewDesc, &pDepthStencilView_))) 
	{
		return false;
	}

	pDeviceContext_->OMSetRenderTargets(1, &pRenderTargetView_, pDepthStencilView_);

	//�r���[�|�[�g�̐ݒ�
	viewport_.Width  = Windows::WIDTH;
	viewport_.Height = Windows::HEIGHT;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	pDeviceContext_->RSSetViewports(1, &viewport_);

	return true;
}

// �㏈��
void DirectX11::Uninit(void)
{
	// �����_�[�^�[�Q�b�g�̏���
	UninitDeletePtr(renderTarget_);
	// ���b�p�[�̏���
	UninitDeletePtr(wrapper_);

	ReleasePtr(pDepthStencilView_);
	ReleasePtr(pRenderTargetView_);
	ReleasePtr(pSwapChain_);
	ReleasePtr(pDeviceContext_);
	ReleasePtr(pDevice_);
}

// �`��J�n
HRESULT DirectX11::DrawBegin(void)
{
	if (!renderTarget_) { return E_FAIL; }

	renderTarget_->ClearRendererTarget(RenderTarget::List::DEFAULT, COLOR::RGBA(32, 100, 92, 255));

	return S_OK;
}

// �`��I��
void DirectX11::DrawEnd(void)
{
	if (renderTarget_)
	{
		renderTarget_->DrawShadowMap();
	}
	HRESULT h = pSwapChain_->Present(1, 0);

	if (h == DXGI_ERROR_DRIVER_INTERNAL_ERROR)
	{
		__debugbreak();
	}
}
