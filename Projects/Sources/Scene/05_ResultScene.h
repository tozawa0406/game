/*
 * @file		ResultScene.h
 * @brief		���U���g�V�[��
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _RESULT_SCENE_H_
#define _RESULT_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

class ResultScene : public BaseScene, public GUI
{
public:
	/* @brief	�R���X�g���N�^		*/
	ResultScene(void);
	/* @brief	�f�X�g���N�^		*/
	~ResultScene(void);

	
	
	/* @brief	����������
	 * @param	(sceneNum)	�V�[���ԍ�
	 * @return	�Ȃ�				*/
	void Init(SceneList sceneNum) override;

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�V�[���J�ڃt���O	*/
	SceneList Update(void) override;

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

	//! �t���[���J�E���^
	int frameCnt_;
	//! �uplease press�v
	CanvasRenderer::Text press_;
	//! �uEnter�v�u�Z�v�uX�v
	CanvasRenderer::Text button_;
	//! �uThank you for Playing!�v
	CanvasRenderer::Text thanks_;
	//! �w�i
	CanvasRenderer::Image back_;
};

#endif // _RESULT_SCENE_H_
