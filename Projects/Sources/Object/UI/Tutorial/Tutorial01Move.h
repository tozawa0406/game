/*
 * @file		Tutorial01Move.h
 * @brief		�ړ��`���[�g���A��
 * @author		���V�đ�
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_01_MOVE_H_
#define _TUTORIAL_01_MOVE_H_

#include "TutorialBase.h"

class Tutorial01Move : public TutorialBase
{
public:
	/* @brief	�R���X�g���N�^		*/
	Tutorial01Move(void);
	/* @brief	�f�X�g���N�^		*/
	~Tutorial01Move(void);

	/* @brief	����������			*/
	void			Init(TutorialManager* manager, Controller* ctrl) override;
	/* @brief	�㏈��				*/
	void			Uninit(void)			override;
	/* @brief	�X�V����			*/
	TutorialBase*	Update(void)			override;

private:
	//! �X�e�B�b�NUI�̃|�C���^
	CanvasRenderer::Image* stick_;
	//! AWSD 0:a 1:w 2:s 3:d
	CanvasRenderer::Image key_[4];

};

#endif // _TUTORIAL_01_MOVE_H_