/*
 * @file		DragonHit.h
 * @brief		���̃_���[�W����
 * @author		���V�đ�
 * @data		2018/11/06
 */
#ifndef _DRAGON_HIT_H_
#define _DRAGON_HIT_H_

#include "../../MonsterAttack.h"

class DragonHit : public MonsterAttack
{
public:
	/* @brief	�R���X�g���N�^		*/
	DragonHit(void);
	/* @brief	�f�X�g���N�^		*/
	~DragonHit(void);
	
	/* @brief	����������
	 * @param	(monster)	�����蔻��̐e�N���X�ɓo�^
	 * @return	�Ȃ�				*/
	void Init(GameObject* monster) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void)  override;

	/* @brief	���s���鏀��
	 * @param	�Ȃ�
	 * @return	�Ȃ�
	 * @detail	���̍U�����L�̃A�j���[�V�����A���x��ݒ肷��		*/
	void SetMove(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�U�����I��������true
	 * @detail	�p���ύX�͂Ȃ��A�ړ����Ȃ��悤���x��0�ɌŒ�
				��莞�Ԍo�ߌ�A�A�j���[�V�����̑��x���グ��A�A�j���[�V�������I�������猳�ɖ߂��ďI��		*/
	bool Update(void)  override;
	
	/* @brief	���[�V�����̏I������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void EndMove(void) override;	
};

#endif // _DRAGON_HIT_H_
