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
	/* @brief	�R���X�g���N�^		*/
	DragonMoveController(void);
	/* @brief	�f�X�g���N�^		*/
	~DragonMoveController(void);


	
	/* @brief	������
	 * @param	(parent)	����������
	 * @return	�Ȃ�				*/
	void Init(Dragon* parent);
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void);
	
	/* @brief	�s������
	 * @param	(act)	�s���t���O
	 * @param	(flag)	��ԃt���O
	 * @return	�Ȃ�				*/
	void Action(int& act, uint& flag);
	
	/* @brief	�W�I�̐ݒ�
	 * @param	(target)	�W�I�̃|�C���^	*/
	inline void SetTarget(GameObject* target) { target_ = target; }
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void GuiUpdate(void) override;

private:
	Dragon*			parent_;	//! �e�̃|�C���^
	GameObject*		target_;	//! �W�I�̃|�C���^

	VECTOR3 dir_;		//! ����
	bool	isDash_;	//! �_�b�V���t���O
	int		cnt_;		//! �J�E���^
};

#endif // _DRAGON_MOVE_CONTROLLER_H_
