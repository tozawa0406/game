/*
 * @file		TutorialFormal.h
 * @brief		アイテムチュートリアル
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_FORMAL_H_
#define _TUTORIAL_FORMAL_H_

#include "TutorialBase.h"

struct SIMPLE_MANUAL
{
	CanvasRenderer::Image button;
	CanvasRenderer::Text  name;

public:
	inline void SetEnable(bool enable) { button.SetEnable(enable); name.SetEnable(enable); }
};

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

	/* @brief	アイコンの設定
	 * @param	(num)		設定する数
	 * @param	(pattern)	設定の数だけ描画位置
	 * @param	(word)		設定の数だけ名前を		*/
	void SetIcon(int num, float* pattern, string* word);

	/* @brief	アイコンを消す			*/
	void NoIconState(void);
	/* @brief	納刀待機時のアイコン	*/
	void WaitState(void);
	/* @brief	納刀歩き時のアイコン	*/
	void WalkState(void);
	/* @brief	走り時のアイコン		*/
	void RunState(void);

	//! プレイヤー
	Player*	player_;
	//! 簡易チュートリアル
	SIMPLE_MANUAL manual_[4];
	//! コントローラのタイプによるパターンのズレ
	float ctrlTypeIcon_;
};

#endif // _TUTORIAL_FORMAL_H_
