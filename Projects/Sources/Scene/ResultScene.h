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
	ResultScene(SceneManager* manager);
	~ResultScene(void);

	void Init(void)   override;
	void Uninit(void) override;
private:
	SceneList Update(void) override;

	void		Flashing(void);
	void		JudgeCtrlType(Controller& ctrl);
	SceneList	EndScene(Controller& ctrl);

	//! �t���[���J�E���^
	int frameCnt_;
	//! �uplease press�v
	CanvasRenderer::Text press_;
	//! �uEnter�v�u�Z�v�uX�v
	CanvasRenderer::Text button_;

};

#endif // _RESULT_SCENE_H_
