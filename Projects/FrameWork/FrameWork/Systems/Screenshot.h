/*
 * @file		Screenshot.h
 * @brief		�X�N���[���V���b�g�̊Ǘ�
 * @author		���V�đ�
 * @data		2018/12/05
 */
#ifndef _SCREENSHOT_H_
#define _SCREENSHOT_H_

#include "../Define/Define.h"
#include "BaseManager.h"
#include "../Graphics/RenderTarget.h"
#include "../Graphics/Wrapper.h"

class Screenshot : public Interface
{
public:
	Screenshot(Systems* systems);
	~Screenshot(void) override;

	HRESULT Init(void)   override;
	void    Update(void) override;
	void    Draw(void)   override;

private:
	void CreateTexture(void);

	int				screenUICnt_;		//! UI�`��̎���
	RenderTarget*	renderTarget_;		//! �����_�[�^�[�Q�b�g�ւ̃|�C���^
	Wrapper*		wrapper_;			//! �`��p�̃��b�p�[�ւ̃|�C���^
};

#endif // _SCREENSHOT_H_