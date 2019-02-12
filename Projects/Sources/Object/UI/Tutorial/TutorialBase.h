/*
 * @file		TutorialBase.h
 * @brief		�`���[�g���A���̊��N���X
 * @author		���V�đ�
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_BASE_H_
#define _TUTORIAL_BASE_H_

#include "TutorialManager.h"

class TutorialBase
{
	// �N���A���o�̏I���t���[����
	static constexpr int END_EFFECT = -60;
public:
	/* @brief	�R���X�g���N�^		*/
	TutorialBase(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~TutorialBase(void);

	/* @brief	����������
	 * @param	(manager)	�}�l�[�W���[
	 * @param	(ctrl)		�R���g���[��
	 * @return	�Ȃ�				*/
	virtual void Init(TutorialManager* manager, Controller* ctrl);

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	virtual void Uninit(void);

	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	���̃`���[�g���A��
	 * @detail	�h�����UpdateTimer���ĂԂ悤�ɁI	*/
	virtual TutorialBase* Update(void) = 0;

protected:
	/* @brief	�^�C�}�[�̍X�V����
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void UpdateTimer(void);

	/* @brief	�`���[�g���A���̏I��
	 * @param	�Ȃ�
	 * @return	�I�������true		*/
	inline bool Finish(void) { return (cnt_ <= END_EFFECT) ? true : false; }

	// ��������
	int			maxCnt_;
	// ���ݎ���
	int			cnt_;
	// �}�l�[�W���[
	TutorialManager*	manager_;
	// �R���g���[���̃|�C���^
	Controller*			ctrl_;
	//! �Z���t
	CanvasRenderer::Text text_;


	// �i�s�x�Q�[�W
	CanvasRenderer::Image gauge_;
	// �Q�[�W�̔w�i
	CanvasRenderer::Image gaugeBack_;

	//! @def	�Z���t�̕����T�C�Y
	static constexpr int FONT_SIZE = 20;
	//! @def	�Z���t�ʒu
	static const VECTOR2 TEXT_POSITION;
	//! @def	�Z���t�`��̈�
	static const VECTOR2 TEXT_DRAW_SIZE;

	//! @def	�����F
	static const COLOR COLOR_PUSH;
	//! @def	�ҋ@�F
	static const COLOR COLOR_WAIT;
	//! @def	��
	static const COLOR COLOR_WHITE;
};

#endif // _TUTORIAL_BASE_H_
