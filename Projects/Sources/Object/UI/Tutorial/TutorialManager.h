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
#include <FrameWork/Systems/Renderer/CanvasRenderer/CanvasRendererText.h> 
#include <FrameWork/Object/Object.h>

class Player;
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

	/* @brief	�`���[�g���A���J�n
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void StartTutorial(void);

	/* @brief	�`���[�g���A�����s��Ȃ�
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void SkipTutorial(void);

	/* @brief	�X�e�B�b�NUI�̃|�C���^���擾
	 * @param	(right)		�E���Ȃ�true
	 * @return	�X�e�B�b�NUI�̃|�C���^�\		*/
	inline CanvasRenderer::Image* GetStickUIPtr(bool right) { return &ctrlStick_[(!right) ? 0 : 1]; }

	/* @brief	�v���C���[�̐ݒ�
	 * @param	(player)	�v���C���[
	 * @return	�Ȃ�				*/
	inline void		SetPlayer(Player* player)	{ player_ = player; }

	/* @brief	�v���C���[�̎擾����
	 * @param	�Ȃ�
	 * @return	�v���C���[			*/
	inline Player*	GetPlayer(void)				{ return player_; }

	/* @brief	�`���[�g���A���̏I��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	inline void End(void) { end_ = true; }

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
	//! �L�����N�^�[
	CanvasRenderer::Image charactor_;
	//! �����o��
	CanvasRenderer::Image baloon_;
	//! �v���C���[
	Player* player_;
	//! �`���[�g���A�����̓R���g���[�����o��
	bool	end_;
};

#endif // _TUTORIAL_MANAGER_H_
