/*
 * @file		Timer.h
 * @brief		�^�C�}�[
 * @author		���V�đ�
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
	int		time_;			//! ���ގ���
	uint8	frame_;			//! �t���[���J�E���^
	uint8	second_;		//! �b

	//! �w�i
	CanvasRenderer::Image back_;
	//! �Q�[�W
	CanvasRenderer::Image circleGauge_;
	//! �I���ʒu�j
	CanvasRenderer::Image handEnd_;
	//! ���݈ʒu�j
	CanvasRenderer::Image handNow_;
	//! ����
	CanvasRenderer::Image clip_;

};

#endif // _TIMER_H_
