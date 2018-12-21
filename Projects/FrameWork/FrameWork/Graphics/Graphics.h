/*
 * @file		Graphics.h
 * @brief		�O���t�B�b�N�XAPI�̏���
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "../Define/Define.h"
#include "Wrapper.h"
#include "RenderTarget.h"

class Graphics
{
	friend class Windows;
	friend class Systems;

public:	
	//! @enum	�O���t�B�b�N�X�̎��
	enum class Type
	{
		UNKNOWN  = -1,
		DirectX9 = 0,
		DirectX11,
		OpenGL,
		MAX
	};	

	//! @def	���𑜓x
	static constexpr int WIDTH  = 1280;
	//! @def	�c�𑜓x
	static constexpr int HEIGHT = 720;

	/* @brief	Windows�N���X�̎擾			*/
	inline Windows*			GetWindow(void)			{ return window_;		}
	/* @brief	Wrapper�N���X�̎擾			*/
	inline Wrapper*			GetWrapper(void)		{ return wrapper_;		}
	/* @brief	RenderTarget�N���X�̎擾	*/
	inline RenderTarget*	GetRenderTarget(void)	{ return renderTarget_; }

	/* @brief	�㏈��		*/
	virtual void	Uninit(void) = 0;

protected:
	/* @brief	�R���X�g���N�^		*/
	Graphics(void) : window_(nullptr) {}

	/* @brief	Windows�N���X�̐ݒ菈��		*/
	void SetWindows(Windows* window) { window_ = window; }

	/* @brief	����������		*/
	virtual HRESULT Init(void)      = 0;
	/* @brief	�`��J�n����	*/
	virtual HRESULT DrawBegin(void) = 0;
	/* @brief	�`��I������	*/
	virtual void	DrawEnd(void)	= 0;

	//! Windows�N���X�̃|�C���^
	Windows*		window_;
	//! Wrapper�N���X�̃|�C���^
	Wrapper*		wrapper_;
	//! RenderTarget�N���X�̃|�C���^
	RenderTarget*	renderTarget_;
};

#endif // _GRAPHICS_H