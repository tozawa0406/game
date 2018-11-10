//-----------------------------------------------------------------------------
//
//	ポーズ[PauseScene.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _PAUSE_SCENE_H_
#define _PAUSE_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

class Pause : public BaseScene, public GUI
{
	// 定数定義
	static constexpr int TITLE_CONTROLLER  = 20000;
	static constexpr int PAUSE_SELECT_UP   = 0;
	static constexpr int PAUSE_SELECT_DOWN = 2;
	static constexpr int PADDING_TIME      = 30;

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

	VECTOR2 pos_[3];		//選択オブジェクトの位置座標
	int		selectNum_;
	int     paddingTimeCnt_;

	CanvasRenderer back_;
	CanvasRenderer menuBack_[3];
	CanvasRenderer notSelect_[2];
	CanvasRenderer menu_[3];
};

#endif // _PAUSE_SCENE_H_
