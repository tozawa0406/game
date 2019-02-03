/*
 * @file		DragonTurn.h
 * @brief		��]�U��
 * @author		���V�đ�
 * @data		2019/02/02
 */
#ifndef _DRAGON_TURN_H_
#define _DRAGON_TURN_H_

#include "../../MonsterAttack.h"

class DragonTurn : public MonsterAttack
{
	//! �U�������蔻��
	enum class Collision : uint8
	{
		WING_LR = 0,
		WING_LC,
		WING_RR,
		WING_RC,
		TAIL_5,
		TAIL_4,
		TAIL_3,
		TAIL_2,
		TAIL_1,
		MAX
	};
public:
	/* @brief	�R���X�g���N�^		*/
	DragonTurn(void);
	/* @brief	�f�X�g���N�^		*/
	~DragonTurn(void);


	
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
	//! �����蔻��
	Collider3D::OBB* collider_[static_cast<int>(Collision::MAX)];
};

#endif // _DRAGON_TURN_H_
