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
public:
	/* @brief	コンストラクタ		*/
	TutorialBase(void) : maxCnt_(0), cnt_(0), ctrl_(nullptr) {}
	/* @brief	デストラクタ		*/
	virtual ~TutorialBase(void) {}

	/* @brief	初期化処理
	 * @param	(manager)	マネージャー
	 * @param	(ctrl)		コントローラ
	 * @return	なし				*/
	virtual void Init(TutorialManager* manager, Controller* ctrl) { manager_ = manager; ctrl_ = ctrl; }

	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	virtual void Uninit(void) = 0;

	/* @brief	更新処理
	 * @param	なし
	 * @return	次のチュートリアル	*/
	virtual TutorialBase* Update(void) = 0;

protected:
	inline bool Finish(void) { return (cnt_ >= maxCnt_) ? true : false; }

	int			maxCnt_;
	int			cnt_;
	TutorialManager*	manager_;
	Controller*			ctrl_;
};

#endif // _TUTORIAL_BASE_H_
