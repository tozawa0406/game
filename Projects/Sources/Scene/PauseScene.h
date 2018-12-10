/*
 * @file		PauseScene.h
 * @brief		ポーズシーン
 * @author		戸澤翔太
 * @data		2018/08/18
 */
#ifndef _PAUSE_SCENE_H_
#define _PAUSE_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

class Pause : public BaseScene, public GUI
{
	static constexpr int SELECT_NUM = 3;
public:
	static constexpr float PAUSE_W = (float)Graphics::WIDTH  / 3;		//幅
	static constexpr float PAUSE_H = (float)Graphics::HEIGHT / 7;		//高さ

	Pause(SceneManager* manager);
	~Pause(void);

	void Init(void)   override;
	void Uninit(void) override;
	SceneList Update(void) override;		//更新処理

private:
	void EnableUI(bool enable);

	VECTOR2 pos_[SELECT_NUM];		//! 選択オブジェクトの位置座標
	int		selectNum_;				//! 選択している番号
	int     paddingTimeCnt_;		//! 選択後の時間経過

	CanvasRenderer::Image back_;						//! 背景
	CanvasRenderer::Image menuBack_[SELECT_NUM];		//! UI背景
	CanvasRenderer::Image notSelect_[SELECT_NUM - 1];	//! 非選択オブジェクトを暗くする
	CanvasRenderer::Image menu_[SELECT_NUM];			//! メニュー
};

#endif // _PAUSE_SCENE_H_
