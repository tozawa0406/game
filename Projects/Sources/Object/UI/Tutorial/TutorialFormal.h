/*
 * @file		TutorialFormal.h
 * @brief		�A�C�e���`���[�g���A��
 * @author		���V�đ�
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_FORMAL_H_
#define _TUTORIAL_FORMAL_H_

#include "TutorialBase.h"

struct SIMPLE_MANUAL
{
	CanvasRenderer::Image button;
	CanvasRenderer::Text  name;

public:
	inline void SetEnable(bool enable) { button.SetEnable(enable); name.SetEnable(enable); }
};

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

	/* @brief	�A�C�R���̐ݒ�
	 * @param	(num)		�ݒ肷�鐔
	 * @param	(pattern)	�ݒ�̐������`��ʒu
	 * @param	(word)		�ݒ�̐��������O��		*/
	void SetIcon(int num, float* pattern, string* word);

	/* @brief	�A�C�R��������			*/
	void NoIconState(void);
	/* @brief	�[���ҋ@���̃A�C�R��	*/
	void WaitState(void);
	/* @brief	�[���������̃A�C�R��	*/
	void WalkState(void);
	/* @brief	���莞�̃A�C�R��		*/
	void RunState(void);

	//! �v���C���[
	Player*	player_;
	//! �ȈՃ`���[�g���A��
	SIMPLE_MANUAL manual_[4];
	//! �R���g���[���̃^�C�v�ɂ��p�^�[���̃Y��
	float ctrlTypeIcon_;
};

#endif // _TUTORIAL_FORMAL_H_
