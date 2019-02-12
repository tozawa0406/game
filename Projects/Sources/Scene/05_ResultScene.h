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
	/* @brief	コンストラクタ		*/
	ResultScene(void);
	/* @brief	デストラクタ		*/
	~ResultScene(void);

	
	
	/* @brief	初期化処理
	 * @param	(sceneNum)	シーン番号
	 * @return	なし				*/
	void Init(SceneList sceneNum) override;

	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	シーン遷移フラグ	*/
	SceneList Update(void) override;

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

	//! フレームカウンタ
	int frameCnt_;
	//! 「please press」
	CanvasRenderer::Text press_;
	//! 「Enter」「〇」「X」
	CanvasRenderer::Text button_;
	//! 「Thank you for Playing!」
	CanvasRenderer::Text thanks_;
	//! 背景
	CanvasRenderer::Image back_;
};

#endif // _RESULT_SCENE_H_
