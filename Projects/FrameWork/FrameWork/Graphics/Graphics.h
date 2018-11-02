//-----------------------------------------------------------------------------
//
//	�O���t�B�b�N�XAPI[Graphics.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "../Define/Define.h"
#include "Wrapper.h"

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class Graphics
{
	friend class Windows;
	friend class Systems;

public:	
	// �O���t�B�b�N�X�̃^�C�v
	enum class Type
	{
		UNKNOWN  = -1,
		DirectX9 = 0,
		DirectX11,
		OpenGL,
		MAX
	};	

	// �𑜓x
	static constexpr int WIDTH  = 1280;
	static constexpr int HEIGHT = 720;

	Windows* GetWindow(void)  { return window_;  }		// �E�B���h�E�N���X�̎󂯓n��
	Wrapper* GetWrapper(void) { return wrapper_; }		// ���b�p�[�N���X�̎󂯓n��

protected:
	Graphics(Windows* window) { window_ = window; }		// �R���X�g���N�^

	virtual HRESULT Init(void)      = 0;	// ����������
	virtual void	Uninit(void)	= 0;	// �㏈��
	virtual HRESULT DrawBegin(void)	= 0;	// �`��J�n
	virtual void	DrawEnd(void)	= 0;	// �`��I��

	virtual void    ClearRenderer(void) {}	// ��ʂ̃N���A

	Windows* window_;		// �E�B���h�E�N���X�ւ̃|�C���^
	Wrapper* wrapper_;		// ���b�p�[�N���X
};

#endif // _GRAPHICS_H