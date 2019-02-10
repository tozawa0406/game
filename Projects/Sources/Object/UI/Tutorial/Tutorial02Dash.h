/*
 * @file		Tutorial02Dash.h
 * @brief		�ړ��`���[�g���A��
 * @author		���V�đ�
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_02_DASH_H_
#define _TUTORIAL_02_DASH_H_

#include "TutorialBase.h"

class Tutorial02Dash : public TutorialBase
{
public:
	/* @brief	�R���X�g���N�^		*/
	Tutorial02Dash(void);
	/* @brief	�f�X�g���N�^		*/
	~Tutorial02Dash(void);

	/* @brief	����������			*/
	void			Init(TutorialManager* manager, Controller* ctrl) override;
	/* @brief	�㏈��				*/
	void			Uninit(void)			override;
	/* @brief	�X�V����			*/
	TutorialBase*	Update(void)			override;

private:
	/* @brief	�R���g���[���̈Ⴂ
	 * @sa		Update()
	 * @param	(type)	�R���g���[���̎��
	 * @return	�Ȃ�				*/
	void InputDifference(uint8 type);

	//! �X�e�B�b�NUI�̃|�C���^
	CanvasRenderer::Image* stick_;
	//! �_�b�V���L�[
	CanvasRenderer::Image dash_;
	//! AWSD 0:a 1:w 2:s 3:d
	CanvasRenderer::Image key_[4];
};

#endif // _TUTORIAL_02_DASH_H_
