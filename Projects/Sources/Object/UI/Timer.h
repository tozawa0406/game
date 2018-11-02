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

class Timer : public Object
{
public:
	Timer(void);
	~Timer(void);

	void Init(void)   override;
	void Uninit(void) override;
	void Update(void) override;

private:
	int time_;			//! 刻む時間
	uint8 frame_;		//! フレームカウンタ
	uint8 second_;		//! 秒

	//! 背景
	CanvasRenderer back_;
	//! ゲージ
	CanvasRenderer circleGauge_;
	//! 終了位置針
	CanvasRenderer handEnd_;
	//! 現在位置針
	CanvasRenderer handNow_;
	//! 中央
	CanvasRenderer clip_;

};

#endif // _TIMER_H_
