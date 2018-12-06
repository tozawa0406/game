/*
 * @file		TitleScene.h
 * @brief		タイトルシーン
 * @author		戸澤翔太
 * @data		2018/08/18
 */
#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

class TitleScene : public BaseScene, public GUI
{
	//! @def	コントローラのタイプによる配列番号
	enum class InputType : uint8
	{
		Keyboard = 0,
		PS4,
		X,

		MAX
	};
public:
	TitleScene(SceneManager* manager);
	~TitleScene(void);

	void Init(void)   override;
	void Uninit(void) override;

	void GuiUpdate(void) override;

private:
	SceneList	Update(void) override;

	void		Flashing(Controller& ctrl);
	int 		JudgeCtrlType(Controller& ctrl);
	SceneList	EndScene(Controller& ctrl);

	//! フレームのカウンタ
	int				frameCnt_;
	//! 「please press」
	CanvasRenderer	press_;
	//! 「Enter」「〇」「X」
	CanvasRenderer	button_[static_cast<uint8>(InputType::MAX)];
};

#endif // _TITLE_SCENE_H_
