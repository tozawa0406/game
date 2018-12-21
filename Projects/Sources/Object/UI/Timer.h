/*
 * @file		Timer.h
 * @brief		タイマー
 * @author		戸澤翔太
 * @data		2018/10/30
 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Timer : public Object, public GUI
{
public:
	/* @brief	コンストラクタ		*/
	Timer(void);
	/* @brief	デストラクタ		*/
	~Timer(void);


	
	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void)   override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	なし
	 * @detail	時間の計測、描画の更新		*/
	void Update(void) override;
	
	/* @brief	Gui更新処理
	 * @param	なし
	 * return	なし				*/
	void GuiUpdate(void) override;

private:
	int		time_;			//! 刻む時間
	uint8	frame_;			//! フレームカウンタ
	uint8	second_;		//! 秒

	//! 背景
	CanvasRenderer::Image back_;
	//! ゲージ
	CanvasRenderer::Image circleGauge_;
	//! 終了位置針
	CanvasRenderer::Image handEnd_;
	//! 現在位置針
	CanvasRenderer::Image handNow_;
	//! 中央
	CanvasRenderer::Image clip_;
};

#endif // _TIMER_H_
