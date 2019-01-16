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
	//! @def	選択肢の数
	static constexpr int SELECT_NUM = 3;
public:
	//! @def	選択肢の幅
	static constexpr float PAUSE_W = Windows::WIDTH  / 3.0f;
	//! @def	選択肢の高
	static constexpr float PAUSE_H = Windows::HEIGHT / 7.0f;

	/* @brief	コンストラクタ		*/
	Pause(void);
	/* @brief	デストラクタ		*/
	~Pause(void);



	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void)   override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	シーン遷移フラグ	*/
	SceneList Update(void) override;

private:
	/* @brief	UIの使用状態を一括変更処理
	 * @param	(enable)	使用状態
	 * @return	なし					*/
	void EnableUI(bool enable);

	VECTOR2 pos_[SELECT_NUM];		//! 選択オブジェクトの位置座標
	int		selectNum_;				//! 選択している番号
	int     paddingTimeCnt_;		//! 選択後の時間経過

	CanvasRenderer::Image back_;						//! 背景
	CanvasRenderer::Image menuBack_[SELECT_NUM];		//! UI背景
	CanvasRenderer::Image notSelect_[SELECT_NUM - 1];	//! 非選択オブジェクトを暗くする
	CanvasRenderer::Text  menu_[SELECT_NUM];			//! メニュー
};

#endif // _PAUSE_SCENE_H_
