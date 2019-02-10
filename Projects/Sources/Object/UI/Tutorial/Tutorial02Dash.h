/*
 * @file		Tutorial02Dash.h
 * @brief		移動チュートリアル
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_02_DASH_H_
#define _TUTORIAL_02_DASH_H_

#include "TutorialBase.h"

class Tutorial02Dash : public TutorialBase
{
public:
	/* @brief	コンストラクタ		*/
	Tutorial02Dash(void);
	/* @brief	デストラクタ		*/
	~Tutorial02Dash(void);

	/* @brief	初期化処理			*/
	void			Init(TutorialManager* manager, Controller* ctrl) override;
	/* @brief	後処理				*/
	void			Uninit(void)			override;
	/* @brief	更新処理			*/
	TutorialBase*	Update(void)			override;

private:
	/* @brief	コントローラの違い
	 * @sa		Update()
	 * @param	(type)	コントローラの種類
	 * @return	なし				*/
	void InputDifference(uint8 type);

	//! スティックUIのポインタ
	CanvasRenderer::Image* stick_;
	//! ダッシュキー
	CanvasRenderer::Image dash_;
	//! AWSD 0:a 1:w 2:s 3:d
	CanvasRenderer::Image key_[4];
};

#endif // _TUTORIAL_02_DASH_H_
