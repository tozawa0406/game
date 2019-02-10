/*
 * @file		TutorialManager.h
 * @brief		チュートリアルの管理
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_MANAGER_H_
#define _TUTORIAL_MANAGER_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/Renderer/CanvasRenderer/CanvasRendererImage.h> 
#include <FrameWork/Object/Object.h>

class TutorialBase;
class TutorialManager : public Object
{
public:
	//! @def	UIポジション
	static const VECTOR2 POSITION;
	//! @def	UIポジション(Keyboard)
	static const VECTOR2 POSITION_KEYBOARD;

	//! @def	UIの大きさ(Keyboard)
	static const VECTOR2 SIZE_KEY;
	//! @def	UIの大きさ(PS4)
	static const VECTOR2 SIZE_PS4;
	//! @def	UIの大きさ(X)
	static const VECTOR2 SIZE_X;

	/* @brief	コンストラクタ		*/
	TutorialManager(void);
	/* @brief	デストラクタ		*/
	~TutorialManager(void);

	/* @brief	初期化処理			*/
	void Init(void)	  override;
	/* @brief	後処理				*/
	void Uninit(void) override;
	/* @brief	更新処理			*/
	void Update(void) override;

	/* @brief	スティックUIのポインタを取得
	 * @param	(right)		右側ならtrue
	 * @return	スティックUIのポインタ―		*/
	inline CanvasRenderer::Image* GetStickUIPtr(bool right) { return &ctrlStick_[(!right) ? 0 : 1]; }

private:
	/* @brief	コントローラの種類判定
	 * @sa		Update()
	 * @param	なし
	 * @return	なし				*/
	void JudgeCtrlType(void);

	//! チュートリアル
	TutorialBase* tutorial_;
	//! 大元
	CanvasRenderer::Image ctrlImage_;
	//! スティック
	CanvasRenderer::Image ctrlStick_[2];
};

#endif // _TUTORIAL_MANAGER_H_
