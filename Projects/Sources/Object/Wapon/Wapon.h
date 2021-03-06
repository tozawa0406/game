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

#include "../GameObject/AttackManager.h"

class Wapon : public Object
{
public:
	Wapon(void) : Object(ObjectTag::WAPON), collider_(nullptr), body_(nullptr), hand_(nullptr), effectRotation_(0), offensivePower_(0), baseOffensivePower_(0) {}
	virtual ~Wapon(void) {}

	/* @brief	²[			*/
	void Setup(bool setup) { (!setup) ? DrawnSword() : PaidSword(); }

	/* @brief	υ
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

	/* @brief	ρ]Μέθ
	 * @param	ρ]						*/
	inline void SetRotation(float rotation) { effectRotation_ = rotation; }

	/* @brief	UJn				*/
	inline void AttackStart(void) 
	{
		if (collider_) { collider_->SetEnable(true); } 

		if (!attackManager_) { return; }
		attackID_ = attackManager_->CreateAttackID();
	}

	/* @brief	UIΉ				*/
	inline void AttackEnd(void)   { if (collider_) { collider_->SetEnable(false); } }

	/* @brief	U©					*/
	inline bool IsAttack(void) { return collider_->IsEnable(); }

	/* @brief	UΗNXΜέθ		*/
	inline void SetAttackManager(AttackManager* attackManager) { attackManager_ = attackManager; }

	inline void SetOffensivePower(int offensivePower) { offensivePower_ = offensivePower + baseOffensivePower_; }

protected:
	//! [
	virtual void PaidSword(void) = 0;
	//! ²
	virtual void DrawnSword(void) = 0;

	//! UΗNX
	AttackManager* attackManager_;
	//! bV
	MeshRenderer mesh_;
	//! ½θ»θ
	Collider3D::OBB* collider_;

	//! ΜΜ{[Μ|C^([)
	const MATRIX* body_;
	//! θΜ{[Μ|C^(²)
	const MATRIX* hand_;

	//! aΑ½GtFNgΜρ]
	float effectRotation_;
	//! UΝ
	int offensivePower_;
	//! νΜξbUΝ
	int baseOffensivePower_;

	//! OρΜUID
	uint8 attackID_;
};

#endif // _WAPON_H_
