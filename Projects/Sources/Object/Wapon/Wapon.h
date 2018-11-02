/*
 * @file		Wapon.h
 * @brief		νΜξκNX
 * @author		ΛΰVγΔΎ
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
	 * @brief	υ
	 * @param	(transform)	eΜTransform
	 * @param	(body)		ΜΜsρ
	 * @param	(hand)		θΜsρ		*/
	void SetParent(const Transform& transform, const MATRIX* body, const MATRIX* hand) 
	{
		transform_.parent = &transform;
		body_ = body;
		hand_ = hand; 
		this->PaidSword();
	}

	void AttackStart(void) { if (collider_) { collider_->SetEnable(true);  } }
	void AttackEnd(void)   { if (collider_) { collider_->SetEnable(false); } }

protected:
	//! [
	virtual void PaidSword(void) = 0;
	//! ²
	virtual void DrawnSword(void) = 0;

	//! bV
	MeshRenderer mesh_;
	//! ½θ»θ
	Collider3D::OBB* collider_;

	const MATRIX* body_;
	const MATRIX* hand_;
};

#endif // _WAPON_H_
