/*
 * @file		Cannon.h
 * @brief		��C
 * @author		���V�đ�
 * @data		2019/01/15
 */
#ifndef _CANNON_H_
#define _CANNON_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Cannon : public Object
{
public:
	/* @brief	�R���X�g���N�^
	 * @param	(transform)		�����p��
	 * @param	(model)			���f��		*/
	Cannon(void);
	/* @brief	�f�X�g���N�^		*/
	~Cannon(void);
	


	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;

	/* @brief	�X�V����(�����Ȃ����߂Ȃ�)		*/
	void Update(void) override {}
};

#endif // _CANNON_H_
