/*
 * @file		Tutorial01Move.h
 * @brief		移動チュートリアル
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_01_MOVE_H_
#define _TUTORIAL_01_MOVE_H_

#include "TutorialBase.h"

class Tutorial01Move : public TutorialBase
{
public:
	Tutorial01Move(void);
	~Tutorial01Move(void);

	void			Init(TutorialManager* manager, Controller* ctrl) override;
	void			Uninit(void)			override;
	TutorialBase*	Update(void)			override;

private:
	VECTOR2 defaultPosition;
	CanvasRenderer::Image* stick_;

};

#endif // _TUTORIAL_01_MOVE_H_
