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
public:
	/* @brief	コンストラクタ		*/
	TitleScene(void);
	/* @brief	デストラクタ		*/
	~TitleScene(void);


	
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
	 * @return	シーン番号			*/
	SceneList	Update(void) override;
	
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし				*/
	void GuiUpdate(void) override;

private:
	/* @brief	点滅処理
	 * @sa		Update()
	 * @param	なし
	 * @return	なし				*/
	void		Flashing(void);

	/* @brief	コントローラのタイプ判定
	 * @sa		Update()
	 * @param	(ctrl)	コントローラ
	 * @return	なし				*/
	void		JudgeCtrlType(Controller& ctrl);
	
	/* @brief	遷移処理
	 * @sa		Update()
	 * @param	(ctrl)	コントローラ
	 * @return	シーン番号			*/
	SceneList	EndScene(Controller& ctrl);

	//! フレームのカウンタ
	int				frameCnt_;
	//! 「please press」
	CanvasRenderer::Text press_;
	//! 「Enter」「〇」「X」
	CanvasRenderer::Text button_;
	//! 背景
	CanvasRenderer::Image back_;
	//! タイトル
	CanvasRenderer::Text title_;
};

#endif // _TITLE_SCENE_H_
