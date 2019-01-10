/*
 * @file		TitleScene.h
 * @brief		�^�C�g���V�[��
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

class TitleScene : public BaseScene, public GUI
{
public:
	/* @brief	�R���X�g���N�^		*/
	TitleScene(void);
	/* @brief	�f�X�g���N�^		*/
	~TitleScene(void);


	
	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void)   override;

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�V�[���ԍ�			*/
	SceneList	Update(void) override;
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void GuiUpdate(void) override;

private:
	/* @brief	�_�ŏ���
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void		Flashing(void);

	/* @brief	�R���g���[���̃^�C�v����
	 * @sa		Update()
	 * @param	(ctrl)	�R���g���[��
	 * @return	�Ȃ�				*/
	void		JudgeCtrlType(Controller& ctrl);
	
	/* @brief	�J�ڏ���
	 * @sa		Update()
	 * @param	(ctrl)	�R���g���[��
	 * @return	�V�[���ԍ�			*/
	SceneList	EndScene(Controller& ctrl);

	//! �t���[���̃J�E���^
	int				frameCnt_;
	//! �uplease press�v
	CanvasRenderer::Text press_;
	//! �uEnter�v�u�Z�v�uX�v
	CanvasRenderer::Text button_;
	//! �w�i
	CanvasRenderer::Image back_;
	//! �^�C�g��
	CanvasRenderer::Text title_;
};

#endif // _TITLE_SCENE_H_
