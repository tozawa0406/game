/*
 * @file		Tutorial06Attack.h
 * @brief		�A�C�e���`���[�g���A��
 * @author		���V�đ�
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_06_ATTACK_H_
#define _TUTORIAL_06_ATTACK_H_

#include "TutorialBase.h"

class Tutorial06Attack : public TutorialBase
{
public:
	/* @brief	�R���X�g���N�^		*/
	Tutorial06Attack(void);
	/* @brief	�f�X�g���N�^		*/
	~Tutorial06Attack(void);

	/* @brief	����������			*/
	void			Init(TutorialManager* manager, Controller* ctrl) override;
	/* @brief	�㏈��				*/
	void			Uninit(void)			override;
	/* @brief	�X�V����			*/
	TutorialBase*	Update(void)			override;

private:
	/* @brief	�R���g���[���̈Ⴂ
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void JedgeCtrlType(void);

	/* @brief	�`���[�g���A������
	 * @param	(anim)	�v���C���[�̃A�j���[�V����
	 * @return	�N���A�Ȃ�true		*/
	bool AddCount(int playerAnim);

	//! �{�^��
	CanvasRenderer::Image key_;
	//! �����G�t�F�N�g
	int		effectCnt_;
	//! �v���C���[
	Player*	player_;
};

#endif // _TUTORIAL_06_ATTACK_H_
