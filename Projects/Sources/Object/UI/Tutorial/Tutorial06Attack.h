/*
 * @file		Tutorial06Attack.h
 * @brief		アイテムチュートリアル
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_06_ATTACK_H_
#define _TUTORIAL_06_ATTACK_H_

#include "TutorialBase.h"

class Tutorial06Attack : public TutorialBase
{
public:
	/* @brief	コンストラクタ		*/
	Tutorial06Attack(void);
	/* @brief	デストラクタ		*/
	~Tutorial06Attack(void);

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

	/* @brief	チュートリアル判定
	 * @param	(anim)	プレイヤーのアニメーション
	 * @return	クリアならtrue		*/
	bool AddCount(int playerAnim);

	//! ボタン
	CanvasRenderer::Image key_;
	//! 押下エフェクト
	int		effectCnt_;
	//! プレイヤー
	Player*	player_;
};

#endif // _TUTORIAL_06_ATTACK_H_
