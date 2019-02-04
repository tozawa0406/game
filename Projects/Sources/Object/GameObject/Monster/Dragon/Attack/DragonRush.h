/*
 * @file		DragonRush.h
 * @brief		���̓ːi
 * @author		���V�đ�
 * @data		2018/11/23
 */
#ifndef _DRAGON_RUSH_H_
#define _DRAGON_RUSH_H_

#include "../../MonsterAttack.h"

class DragonRush : public MonsterAttack
{
	//! @def	�����蔻��
	enum class Collision : uint8
	{
		BODY = 0,
		HEAD,
		NECK1,
		NECK2,
		WING_LL,
		WING_LC,
		WING_LR,
		WING_L_CLAW,
		WING_RL,
		WING_RC,
		WING_RR,
		WING_R_CLAW,
		ARM_L_UP,
		ARM_L_UP2,
		ARM_L_DOWN,
		ARM_R_UP,
		ARM_R_UP2,
		ARM_R_DOWN,
		LEGS_L_UP,
		LEGS_L_UP2,
		LEGS_L_DOWN,
		LEGS_R_UP,
		LEGS_R_UP2,
		LEGS_R_DOWN,
		MAX
	};

public:
	/* @brief	�R���X�g���N�^		*/
	DragonRush(void);
	/* @brief	�f�X�g���N�^		*/
	~DragonRush(void);
	
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
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�
	 * @detail	�U�����I�u�W�F�N�g����Ă΂��		*/
	void GuiUpdate(void) override;

private:
	//! �����̃A�j���[�V�������g���̂ŃJ�E���^���K�v
	int cnt_;
	//! �����蔻��
	Collider3D::OBB* collider_[static_cast<int>(Collision::MAX)];
};

#endif // _DRAGON_RUSH_H_
