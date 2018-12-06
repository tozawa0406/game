/*
 * @file		DragonMoveController.h
 * @brief		�G�̓����̐���
 * @author		���V�đ�
 * @data		2018/11/15
 */
#ifndef _DRAGON_MOVE_CONTROLLER_H_
#define _DRAGON_MOVE_CONTROLLER_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include "../../GameObject.h"

class Dragon;
class DragonMoveController : public GUI
{
public:
	DragonMoveController(void);
	~DragonMoveController(void);

	void Init(Dragon* parent);
	void Uninit(void);

	void GuiUpdate(void) override;

	/* @brief	�W�I�̐ݒ�
	 * @param	(target)	�W�I�̃|�C���^	*/
	inline void SetTarget(GameObject* target) { target_ = target; }

	void Action(int& act, uint& flag);


private:
	Dragon*			parent_;	//! �e�̃|�C���^
	GameObject*		target_;	//! �W�I�̃|�C���^

	VECTOR3 dir_;		//! ����
	bool	isDash_;	//! �_�b�V���t���O
	int		cnt_;		//! �J�E���^
};

#endif // _DRAGON_MOVE_CONTROLLER_H_
