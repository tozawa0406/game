/*
 * @file		Tutorial05Item.h
 * @brief		アイテムチュートリアル
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_05_ITEM_H_
#define _TUTORIAL_05_ITEM_H_

#include "TutorialBase.h"

class Tutorial05Item : public TutorialBase
{
public:
	/* @brief	コンストラクタ		*/
	Tutorial05Item(void);
	/* @brief	デストラクタ		*/
	~Tutorial05Item(void);

	/* @brief	初期化処理			*/
	void			Init(TutorialManager* manager, Controller* ctrl) override;
	/* @brief	後処理				*/
	void			Uninit(void)			override;
	/* @brief	更新処理			*/
	TutorialBase*	Update(void)			override;

private:
	/* @brief	コントローラの違い
	 * @sa		Update()
	 * @param	なし
	 * @return	なし				*/
	void JedgeCtrlType(void);

	//! ボタン 0:R 1:T 2:Y
	CanvasRenderer::Image key_[3];
	//! 押下エフェクト
	int effectCnt_;
};

#endif // _TUTORIAL_05_ITEM_H_
