//-----------------------------------------------------------------------------
//
//	DirectX11�̏���[DirectX11.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DIREXT_X11_H_
#define _DIRECT_X11_H_

#include "WICTextureLoader.h"
#include "../Graphics.h"

#pragma warning (disable : 4005)
#pragma warning (disable : 4838)
#include <xnamath.h>
#include <d3d11.h>
#include <d3dx11.h>

#pragma comment(lib, "d3d11.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#endif

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class DirectX11 : public Graphics
{
	friend Windows;
public:
	ID3D11Device*        GetDevice(void)        { return pDevice_;        }		// �f�o�C�X�̎󂯓n��
	ID3D11DeviceContext* GetDeviceContext(void) { return pDeviceContext_; }		// �R���e�L�X�g�̎󂯓n��
	IDXGISwapChain*		 GetSwapChain(void)		{ return pSwapChain_;     }

private:
	DirectX11(Windows* window);

	HRESULT Init(void)      override;		// ����������
	void    Uninit(void)    override;		// �㏈��
	HRESULT DrawBegin(void) override;		// �`��J�n
	void    DrawEnd(void)   override;		// �`��I��

	void ClearRenderer(void) override;		// ��ʂ̃N���A

	HRESULT InitAll(void);			// �S�Ă̏���������
	bool    SetDevice(void);		// �f�o�C�X�̐ݒ�

	IDXGISwapChain*         pSwapChain_;
	ID3D11Device*           pDevice_;				// DirectX�̋@�\
	ID3D11DeviceContext*    pDeviceContext_;		// GPU�̕�
	ID3D11RenderTargetView* pRenderTargetView_;
	ID3D11DepthStencilView* pDepthStencilView_;
};

#endif // _DIRECT_X11_H