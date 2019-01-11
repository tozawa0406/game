/*
 * @file		Cart.h
 * @brief		�艟����
 * @author		���V�đ�
 * @data		2018/01/1
 */
#ifndef _CART_H_
#define _CART_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Cart : public Object
{
public:
	/* @brief	�R���X�g���N�^		*/
	Cart(void);
	/* @brief	�f�X�g���N�^		*/
	~Cart(void);


	
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

#endif // _CART_H_
