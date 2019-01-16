/*
 * @file		Wall.h
 * @brief		�ǂ̃I�u�W�F�N�g�����Ǘ�
 * @author		���V�đ�
 * @data		2019/01/15
 */
#ifndef _WALL_H_
#define _WALL_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Wall : public Object
{
public:
	/* @brief	�R���X�g���N�^		*/
	Wall(void);
	/* @brief	�f�X�g���N�^		*/
	~Wall(void);
	


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

#endif // _WALL_H_