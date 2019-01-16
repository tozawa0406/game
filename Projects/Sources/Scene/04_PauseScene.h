/*
 * @file		PauseScene.h
 * @brief		�|�[�Y�V�[��
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _PAUSE_SCENE_H_
#define _PAUSE_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

class Pause : public BaseScene, public GUI
{
	//! @def	�I�����̐�
	static constexpr int SELECT_NUM = 3;
public:
	//! @def	�I�����̕�
	static constexpr float PAUSE_W = Windows::WIDTH  / 3.0f;
	//! @def	�I�����̍�
	static constexpr float PAUSE_H = Windows::HEIGHT / 7.0f;

	/* @brief	�R���X�g���N�^		*/
	Pause(void);
	/* @brief	�f�X�g���N�^		*/
	~Pause(void);



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
	 * @return	�V�[���J�ڃt���O	*/
	SceneList Update(void) override;

private:
	/* @brief	UI�̎g�p��Ԃ��ꊇ�ύX����
	 * @param	(enable)	�g�p���
	 * @return	�Ȃ�					*/
	void EnableUI(bool enable);

	VECTOR2 pos_[SELECT_NUM];		//! �I���I�u�W�F�N�g�̈ʒu���W
	int		selectNum_;				//! �I�����Ă���ԍ�
	int     paddingTimeCnt_;		//! �I����̎��Ԍo��

	CanvasRenderer::Image back_;						//! �w�i
	CanvasRenderer::Image menuBack_[SELECT_NUM];		//! UI�w�i
	CanvasRenderer::Image notSelect_[SELECT_NUM - 1];	//! ��I���I�u�W�F�N�g���Â�����
	CanvasRenderer::Text  menu_[SELECT_NUM];			//! ���j���[
};

#endif // _PAUSE_SCENE_H_
