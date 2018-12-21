/*
 * @file		PlayerLife.h
 * @brief		�v���C���[�̃��C�t�ƃX�^�~�i
 * @author		���V�đ�
 * @data		2018/11/19
 */
#ifndef _PLAYER_LIFE_H_
#define _PLAYER_LIFE_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

#include "../GameObject/Player/Player.h"

class PlayerLife : public Object
{
public:
	/* @brief	�R���X�g���N�^		*/
	PlayerLife(void);
	/* @brief	�f�X�g���N�^		*/
	~PlayerLife(void);


	
	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void)   override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;

	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Update(void) override;

	/* @brief	�v���C���[�̐ݒ�
	 * @param	(player)	�v���C���[		*/
	inline void SetPlayer(Player* player) { player_ = player; }

private:
	//! �v���C���[�̃|�C���^
	Player* player_;

	//! �w�i
	CanvasRenderer::Image back_;
	//! ���C�t�o�[
	CanvasRenderer::Image life_;
	//! �X�^�~�i�o�[
	CanvasRenderer::Image stamina_;
};

#endif // _PLAYER_LIFE_H_
