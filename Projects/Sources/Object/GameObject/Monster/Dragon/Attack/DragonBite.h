/*
 * @file		DragonBite.h
 * @brief		���̊��݂��U��
 * @author		���V�đ�
 * @data		2018/11/07
 */
#ifndef _DRAGON_BITE_H_
#define _DRAGON_BITE_H_

#include "../../MonsterAttack.h"

class DragonBite : public MonsterAttack
{
public:
	DragonBite(void);
	~DragonBite(void);

	void Init(GameObject* monster) override;
	void Uninit(void)  override;
	void SetMove(void) override;
	bool Update(void)  override;

	void GuiUpdate(void) override;

private:
	Collider3D::OBB* collider_;

	//! �f�b�o�O�@�\�Ŏg�p
	float debug_speed_;
	int   debug_changeFrame_;
};

#endif // _DRAGON_BITE_H_
