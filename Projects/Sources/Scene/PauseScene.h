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
	static constexpr int SELECT_NUM = 3;
public:
	static constexpr float PAUSE_W = (float)Graphics::WIDTH  / 3;		//��
	static constexpr float PAUSE_H = (float)Graphics::HEIGHT / 7;		//����

	Pause(SceneManager* manager);
	~Pause(void);

	void Init(void)   override;
	void Uninit(void) override;
	SceneList Update(void) override;		//�X�V����

private:
	void EnableUI(bool enable);

	VECTOR2 pos_[SELECT_NUM];		//! �I���I�u�W�F�N�g�̈ʒu���W
	int		selectNum_;				//! �I�����Ă���ԍ�
	int     paddingTimeCnt_;		//! �I����̎��Ԍo��

	CanvasRenderer::Image back_;						//! �w�i
	CanvasRenderer::Image menuBack_[SELECT_NUM];		//! UI�w�i
	CanvasRenderer::Image notSelect_[SELECT_NUM - 1];	//! ��I���I�u�W�F�N�g���Â�����
	CanvasRenderer::Image menu_[SELECT_NUM];			//! ���j���[
};

#endif // _PAUSE_SCENE_H_
