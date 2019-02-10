/*
 * @file		TutorialManager.h
 * @brief		�`���[�g���A���̊Ǘ�
 * @author		���V�đ�
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_MANAGER_H_
#define _TUTORIAL_MANAGER_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/Renderer/CanvasRenderer/CanvasRendererImage.h> 
#include <FrameWork/Object/Object.h>

class TutorialBase;
class TutorialManager : public Object
{
public:
	//! @def	UI�|�W�V����
	static const VECTOR2 POSITION;
	//! @def	UI�|�W�V����(Keyboard)
	static const VECTOR2 POSITION_KEYBOARD;

	//! @def	UI�̑傫��(Keyboard)
	static const VECTOR2 SIZE_KEY;
	//! @def	UI�̑傫��(PS4)
	static const VECTOR2 SIZE_PS4;
	//! @def	UI�̑傫��(X)
	static const VECTOR2 SIZE_X;

	/* @brief	�R���X�g���N�^		*/
	TutorialManager(void);
	/* @brief	�f�X�g���N�^		*/
	~TutorialManager(void);

	/* @brief	����������			*/
	void Init(void)	  override;
	/* @brief	�㏈��				*/
	void Uninit(void) override;
	/* @brief	�X�V����			*/
	void Update(void) override;

	/* @brief	�X�e�B�b�NUI�̃|�C���^���擾
	 * @param	(right)		�E���Ȃ�true
	 * @return	�X�e�B�b�NUI�̃|�C���^�\		*/
	inline CanvasRenderer::Image* GetStickUIPtr(bool right) { return &ctrlStick_[(!right) ? 0 : 1]; }

private:
	/* @brief	�R���g���[���̎�ޔ���
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void JudgeCtrlType(void);

	//! �`���[�g���A��
	TutorialBase* tutorial_;
	//! �匳
	CanvasRenderer::Image ctrlImage_;
	//! �X�e�B�b�N
	CanvasRenderer::Image ctrlStick_[2];
};

#endif // _TUTORIAL_MANAGER_H_
