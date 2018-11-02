/*
 * @file		Wapon.h
 * @brief		武器の基底クラス
 * @author		戸澤翔太
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
	 * @brief	装備
	 * @param	(transform)	親のTransform
	 * @param	(body)		体の行列
	 * @param	(hand)		手の行列		*/
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
	//! 納刀処理
	virtual void PaidSword(void) = 0;
	//! 抜刀処理
	virtual void DrawnSword(void) = 0;

	//! メッシュ
	MeshRenderer mesh_;
	//! 当たり判定
	Collider3D::OBB* collider_;

	const MATRIX* body_;
	const MATRIX* hand_;
};

#endif // _WAPON_H_
