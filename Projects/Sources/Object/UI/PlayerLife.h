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

class Player;
class PlayerLife : public Object
{
public:
	//! @def	�ő�l
	static constexpr float MAX_LIFE = 150;

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
	void MoveBar(CanvasRenderer::Image& bar, float value, const VECTOR2& offset, const float weight, CanvasRenderer::Image* ornament);

	//! �v���C���[�̃|�C���^
	Player* player_;

	//! �o�[�̗��[�̑���
	CanvasRenderer::Image ornament_[4];
	//! ���C�t�w�i
	CanvasRenderer::Image backLife_;
	//! �X�^�~�i�w�i
	CanvasRenderer::Image backStamina_;
	//! ���C�t�o�[
	CanvasRenderer::Image life_;
	//! �X�^�~�i�o�[
	CanvasRenderer::Image stamina_;
};

#endif // _PLAYER_LIFE_H_
