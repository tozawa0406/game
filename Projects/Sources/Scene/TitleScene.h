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
	TitleScene(SceneManager* manager);
	~TitleScene(void);

	void Init(void)   override;
	void Uninit(void) override;

	void GuiUpdate(void) override;

private:
	SceneList	Update(void) override;

	void		Flashing(void);
	void		JudgeCtrlType(Controller& ctrl);
	SceneList	EndScene(Controller& ctrl);

	//! �t���[���̃J�E���^
	int				frameCnt_;
	//! �uplease press�v
	CanvasRenderer::Text press_;

	//! �uEnter�v�u�Z�v�uX�v
	CanvasRenderer::Text button_;

};

#endif // _TITLE_SCENE_H_
