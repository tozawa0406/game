/*
 * @file		Goal.h
 * @brief		���̃V�[���֑J�ڂ���I�u�W�F�N�g
 * @author		���V�đ�
 * @data		2018/01/18
 */
#ifndef _GOAL_H_
#define _GOAL_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Goal : public Object
{
public:
	/* @brief	�R���X�g���N�^		*/
	Goal(void);
	/* @brief	�f�X�g���N�^		*/
	~Goal(void);


	
	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;

	/* @brief	�X�V����			*/
	void Update(void) override;

	inline bool IsChange(void) { return change_; }

private:
	//! �����蔻��
	Collider3D::OBB*	collider_;
	//! �V�[���J��
	bool				change_;
};

#endif // _GOAL_H_
