/*
 * @file		DragonWingAttack.h
 * @brief		���̗��U��
 * @author		���V�đ�
 * @data		2018/11/07
 */
#ifndef _DRAGON_WING_ATTACK_H_
#define _DRAGON_WING_ATTACK_H_

#include "../../MonsterAttack.h"

class DragonWingAttack : public MonsterAttack
{
	//! @enum	��
	enum class Wing : uint8
	{
		CLAW_L = 0,
		CLAW_R,

		MAX
	};
public:
	DragonWingAttack(void);
	~DragonWingAttack(void);

	void Init(GameObject* monster) override;
	void Uninit(void)  override;
	void SetMove(void) override;
	bool Update(void)  override;
	void EndMove(void) override;

	void GuiUpdate(void) override;

private:
	//! ���̓����蔻��
	Collider3D::OBB* collider_[static_cast<int>(Wing::MAX)];

	//! �f�b�o�O�@�\�Ŏg�p
	float debug_speed_;
	int   debug_changeFrame_;
};

#endif // _DRAGON_WING_ATTACK_H_
