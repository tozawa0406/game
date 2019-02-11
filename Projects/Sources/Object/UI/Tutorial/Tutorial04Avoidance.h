/*
 * @file		Tutorial04Avoidance.h
 * @brief		回避チュートリアル
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_04_AVOIDANCE_H_
#define _TUTORIAL_04_AVOIDANCE_H_

#include "TutorialBase.h"

class Tutorial04Avoidance : public TutorialBase
{
public:
	Tutorial04Avoidance(void);
	~Tutorial04Avoidance(void);

	void			Init(TutorialManager* manager, Controller* ctrl) override;
	void			Uninit(void)			override;
	TutorialBase*	Update(void)			override;

private:
	/* @brief	コントローラの種類
	 * @param	なし
	 * @return	なし			*/
	void JudgeCtrlType(void);

	//! ボタンUI
	CanvasRenderer::Image key_;
	//! 押下エフェクト
	int effectCnt_;
	//! プレイヤー
	Player* player_;
};

#endif // _TUTORIAL_04_AVOIDANCE_H_
