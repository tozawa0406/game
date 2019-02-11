/*
 * @file		TutorialFormal.h
 * @brief		�A�C�e���`���[�g���A��
 * @author		���V�đ�
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_FORMAL_H_
#define _TUTORIAL_FORMAL_H_

#include "TutorialBase.h"

class TutorialFormal : public TutorialBase
{
public:
	/* @brief	�R���X�g���N�^		*/
	TutorialFormal(void);
	/* @brief	�f�X�g���N�^		*/
	~TutorialFormal(void);

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

	//! �v���C���[
	Player*	player_;
};

#endif // _TUTORIAL_FORMAL_H_
