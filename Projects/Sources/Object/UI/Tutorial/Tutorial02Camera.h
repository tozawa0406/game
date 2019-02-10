/*
 * @file		Tutorial02Camera.h
 * @brief		カメラチュートリアル
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_02_CAMERA_H_
#define _TUTORIAL_02_CAMERA_H_

#include "TutorialBase.h"

class Tutorial02Camera : public TutorialBase
{
public:
	Tutorial02Camera(void);
	~Tutorial02Camera(void);

	void			Init(TutorialManager* manager, Controller* ctrl) override;
	void			Uninit(void)			override;
	TutorialBase*	Update(void)			override;

private:
	VECTOR2 defaultPosition;
	CanvasRenderer::Image* stick_;

};

#endif // _TUTORIAL_02_CAMERA_H_
