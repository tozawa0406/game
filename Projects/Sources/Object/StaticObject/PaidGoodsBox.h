/*
 * @file		PaidGoodsBox.h
 * @brief		�x���i��
 * @author		���V�đ�
 * @data		2018/10/30
 */
#ifndef _PAID_GOODS_BOX_H_
#define _PAID_GOODS_BOX_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class PaidGoodsBox : public Object
{
public:
	PaidGoodsBox(void);
	~PaidGoodsBox(void);

	void Init(void) override;
	void Uninit(void) override;

	/* @brief	�X�V����(�����Ȃ����߂Ȃ�)		*/
	void Update(void) override {}

private:
	//! ���b�V��
	MeshRenderer mesh_;
	//! �����蔻��
	Collider3D::OBB* collider_;
};

#endif // _PAID_GOODS_BOX_H_
