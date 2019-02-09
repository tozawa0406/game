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
public:
	/* @brief	�R���X�g���N�^		*/
	TutorialBase(void) : maxCnt_(0), cnt_(0), ctrl_(nullptr) {}
	/* @brief	�f�X�g���N�^		*/
	virtual ~TutorialBase(void) {}

	/* @brief	����������
	 * @param	(manager)	�}�l�[�W���[
	 * @param	(ctrl)		�R���g���[��
	 * @return	�Ȃ�				*/
	virtual void Init(TutorialManager* manager, Controller* ctrl) { manager_ = manager; ctrl_ = ctrl; }

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	virtual void Uninit(void) = 0;

	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	���̃`���[�g���A��	*/
	virtual TutorialBase* Update(void) = 0;

protected:
	inline bool Finish(void) { return (cnt_ >= maxCnt_) ? true : false; }

	int			maxCnt_;
	int			cnt_;
	TutorialManager*	manager_;
	Controller*			ctrl_;
};

#endif // _TUTORIAL_BASE_H_
