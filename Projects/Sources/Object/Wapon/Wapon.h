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
	Wapon(void) : Object(Object::Tag::WAPON), collider_(nullptr), body_(nullptr), hand_(nullptr), isHit_(false), effectRotation_(0) {}
	virtual ~Wapon(void) {}

	/* @brief	抜刀納刀処理			*/
	void Setup(bool setup) { (!setup) ? DrawnSword() : PaidSword(); }

	/* @brief	装備処理
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

	/* @brief	回転の設定処理
	 * @param	回転						*/
	inline void SetRotation(float rotation) { effectRotation_ = rotation; }

	/* @brief	攻撃開始処理				*/
	inline void AttackStart(void) { if (collider_) { collider_->SetEnable(true); } isHit_ = false; }
	/* @brief	攻撃終了処理				*/
	inline void AttackEnd(void)   { if (collider_) { collider_->SetEnable(false); } }

	/* @brief	攻撃中か					*/
	inline bool IsAttack(void) { return collider_->IsEnable(); }

protected:
	//! 納刀処理
	virtual void PaidSword(void) = 0;
	//! 抜刀処理
	virtual void DrawnSword(void) = 0;

	//! メッシュ
	MeshRenderer mesh_;
	//! 当たり判定
	Collider3D::OBB* collider_;

	//! 体のボーンのポインタ(納刀時)
	const MATRIX* body_;
	//! 手のボーンのポインタ(抜刀時)
	const MATRIX* hand_;

	//! 斬ったエフェクトの回転
	float effectRotation_;

	//! 当たった？
	bool isHit_;
};

#endif // _WAPON_H_
