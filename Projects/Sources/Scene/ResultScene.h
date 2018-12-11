/*
 * @file		ResultScene.h
 * @brief		リザルトシーン
 * @author		戸澤翔太
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

	//! フレームカウンタ
	int frameCnt_;
	//! 「please press」
	CanvasRenderer::Text press_;
	//! 「Enter」「〇」「X」
	CanvasRenderer::Text button_;

};

#endif // _RESULT_SCENE_H_
