/*
 * @file		Tutorial04Avoidance.h
 * @brief		����`���[�g���A��
 * @author		���V�đ�
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_04_AVOIDANCE_H_
#define _TUTORIAL_04_AVOIDANCE_H_

#include "TutorialBase.h"

class Tutorial04Avoidance : public TutorialBase
{
public:
	Tutorial04Avoidance(void);
	~Tutorial04Avoidance(void);

	void			Init(TutorialManager* manager, Controller* ctrl) override;
	void			Uninit(void)			override;
	TutorialBase*	Update(void)			override;

private:
	/* @brief	�R���g���[���̎��
	 * @param	�Ȃ�
	 * @return	�Ȃ�			*/
	void JudgeCtrlType(void);

	//! �{�^��UI
	CanvasRenderer::Image key_;
	//! �����G�t�F�N�g
	int effectCnt_;
	//! �v���C���[
	Player* player_;
};

#endif // _TUTORIAL_04_AVOIDANCE_H_
