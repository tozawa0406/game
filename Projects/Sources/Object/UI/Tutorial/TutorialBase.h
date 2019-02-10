/*
 * @file		TutorialBase.h
 * @brief		チュートリアルの基底クラス
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_BASE_H_
#define _TUTORIAL_BASE_H_

#include "TutorialManager.h"

class TutorialBase
{
	// クリア演出の終了フレーム数
	static constexpr int END_EFFECT = -60;
public:
	/* @brief	コンストラクタ		*/
	TutorialBase(void);
	/* @brief	デストラクタ		*/
	virtual ~TutorialBase(void);

	/* @brief	初期化処理
	 * @param	(manager)	マネージャー
	 * @param	(ctrl)		コントローラ
	 * @return	なし				*/
	virtual void Init(TutorialManager* manager, Controller* ctrl);

	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	virtual void Uninit(void);

	/* @brief	更新処理
	 * @param	なし
	 * @return	次のチュートリアル
	 * @detail	派生先でUpdateTimerを呼ぶように！	*/
	virtual TutorialBase* Update(void) = 0;

protected:

	/* @brief	タイマーの更新処理
	 * @sa		Update()
	 * @param	なし
	 * @return	なし				*/
	void UpdateTimer(void);

	/* @brief	チュートリアルの終了
	 * @param	なし
	 * @return	終わったらtrue		*/
	inline bool Finish(void) { return (cnt_ <= END_EFFECT) ? true : false; }

	// 持続時間
	int			maxCnt_;
	// 現在時間
	int			cnt_;
	// マネージャー
	TutorialManager*	manager_;
	// コントローラのポインタ
	Controller*			ctrl_;

	// 進行度ゲージ
	CanvasRenderer::Image gauge_;
	// ゲージの背景
	CanvasRenderer::Image gaugeBack_;
};

#endif // _TUTORIAL_BASE_H_
