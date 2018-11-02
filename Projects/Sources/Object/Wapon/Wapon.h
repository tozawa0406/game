/*
 * @file		Wapon.h
 * @brief		����̊��N���X
 * @author		���V�đ�
 * @data		2018/10/30
 */
#ifndef _WAPON_H_
#define _WAPON_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Wapon : public Object
{
public:
	Wapon(void) : Object(Object::Tag::WAPON), collider_(nullptr), body_(nullptr), hand_(nullptr) {}
	virtual ~Wapon(void) {}

	void Setup(bool setup) { (!setup) ? DrawnSword() : PaidSword(); }

	/* @fn		SetParent
	 * @brief	����
	 * @param	(transform)	�e��Transform
	 * @param	(body)		�̂̍s��
	 * @param	(hand)		��̍s��		*/
	void SetParent(const Transform& transform, const MATRIX* body, const MATRIX* hand) 
	{
		transform_.parent = &transform;
		body_ = body;
		hand_ = hand; 
		this->PaidSword();
	}

	inline void AttackStart(void) { if (collider_) { collider_->SetEnable(true);  } }
	inline void AttackEnd(void)   { if (collider_) { collider_->SetEnable(false); } }

protected:
	//! �[������
	virtual void PaidSword(void) = 0;
	//! ��������
	virtual void DrawnSword(void) = 0;

	//! ���b�V��
	MeshRenderer mesh_;
	//! �����蔻��
	Collider3D::OBB* collider_;

	const MATRIX* body_;
	const MATRIX* hand_;
};

#endif // _WAPON_H_
