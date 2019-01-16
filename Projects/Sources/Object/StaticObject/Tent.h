/*
 * @file		Tent.h
 * @brief		�e���g
 * @author		���V�đ�
 * @data		2018/01/16
 */
#ifndef _TENT_H_
#define _TENT_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Tent : public Object
{
public:
	/* @brief	�R���X�g���N�^		*/
	Tent(void);
	/* @brief	�f�X�g���N�^		*/
	~Tent(void);


	
	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�						*/
	void Uninit(void) override;

	/* @brief	�X�V����(�����Ȃ����߂Ȃ�)		*/
	void Update(void) override {}

private:
	//! ���b�V��
	MeshRenderer mesh_;
	//! �����蔻��
	Collider3D::OBB* collider_;
};

#endif // _TENT_H_
