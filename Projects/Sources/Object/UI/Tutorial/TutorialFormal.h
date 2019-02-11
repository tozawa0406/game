/*
 * @file		TutorialFormal.h
 * @brief		アイテムチュートリアル
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_FORMAL_H_
#define _TUTORIAL_FORMAL_H_

#include "TutorialBase.h"

class TutorialFormal : public TutorialBase
{
public:
	/* @brief	コンストラクタ		*/
	TutorialFormal(void);
	/* @brief	デストラクタ		*/
	~TutorialFormal(void);

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

	//! プレイヤー
	Player*	player_;
};

#endif // _TUTORIAL_FORMAL_H_
