/*
 * @file		Timer.h
 * @brief		^C}[
 * @author		ËāVãÄū
 * @data		2018/10/30
 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Timer : public Object, public GUI
{
public:
	Timer(void);
	~Timer(void);

	void Init(void)   override;
	void Uninit(void) override;
	void Update(void) override;

	void GuiUpdate(void) override;

private:
	int		time_;			//! ÞÔ
	uint8	frame_;			//! t[JE^
	uint8	second_;		//! b

	//! wi
	CanvasRenderer back_;
	//! Q[W
	CanvasRenderer circleGauge_;
	//! IđĘuj
	CanvasRenderer handEnd_;
	//! ŧÝĘuj
	CanvasRenderer handNow_;
	//! 
	CanvasRenderer clip_;

};

#endif // _TIMER_H_
