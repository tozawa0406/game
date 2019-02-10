/*
 * @file		Tutorial03Camera.h
 * @brief		カメラチュートリアル
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_03_CAMERA_H_
#define _TUTORIAL_03_CAMERA_H_

#include "TutorialBase.h"

class Tutorial03Camera : public TutorialBase
{
public:
	Tutorial03Camera(void);
	~Tutorial03Camera(void);

	void			Init(TutorialManager* manager, Controller* ctrl) override;
	void			Uninit(void)			override;
	TutorialBase*	Update(void)			override;

private:
	CanvasRenderer::Image* stick_;
	//! AWSD 0:i 1:j 2:k 3:l
	CanvasRenderer::Image key_[4];
};

#endif // _TUTORIAL_03_CAMERA_H_
