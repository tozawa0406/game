/*
 * @file		Tutorial05Item.h
 * @brief		�A�C�e���`���[�g���A��
 * @author		���V�đ�
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_05_ITEM_H_
#define _TUTORIAL_05_ITEM_H_

#include "TutorialBase.h"

class Tutorial05Item : public TutorialBase
{
public:
	/* @brief	�R���X�g���N�^		*/
	Tutorial05Item(void);
	/* @brief	�f�X�g���N�^		*/
	~Tutorial05Item(void);

	/* @brief	����������			*/
	void			Init(TutorialManager* manager, Controller* ctrl) override;
	/* @brief	�㏈��				*/
	void			Uninit(void)			override;
	/* @brief	�X�V����			*/
	TutorialBase*	Update(void)			override;

private:
	/* @brief	�R���g���[���̈Ⴂ
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void JedgeCtrlType(void);

	//! �{�^�� 0:R 1:T 2:Y
	CanvasRenderer::Image key_[3];
	//! �����G�t�F�N�g
	int effectCnt_;
};

#endif // _TUTORIAL_05_ITEM_H_
