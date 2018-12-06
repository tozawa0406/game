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
	Wapon(void) : Object(Object::Tag::WAPON), collider_(nullptr), body_(nullptr), hand_(nullptr), isHit_(false), effectRotation_(0) {}
	virtual ~Wapon(void) {}

	/* @brief	�����[������			*/
	void Setup(bool setup) { (!setup) ? DrawnSword() : PaidSword(); }

	/* @brief	��������
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

	/* @brief	��]�̐ݒ菈��
	 * @param	��]						*/
	inline void SetRotation(float rotation) { effectRotation_ = rotation; }

	/* @brief	�U���J�n����				*/
	inline void AttackStart(void) { if (collider_) { collider_->SetEnable(true); } isHit_ = false; }
	/* @brief	�U���I������				*/
	inline void AttackEnd(void)   { if (collider_) { collider_->SetEnable(false); } }

	/* @brief	�U������					*/
	inline bool IsAttack(void) { return collider_->IsEnable(); }

protected:
	//! �[������
	virtual void PaidSword(void) = 0;
	//! ��������
	virtual void DrawnSword(void) = 0;

	//! ���b�V��
	MeshRenderer mesh_;
	//! �����蔻��
	Collider3D::OBB* collider_;

	//! �̂̃{�[���̃|�C���^(�[����)
	const MATRIX* body_;
	//! ��̃{�[���̃|�C���^(������)
	const MATRIX* hand_;

	//! �a�����G�t�F�N�g�̉�]
	float effectRotation_;

	//! ���������H
	bool isHit_;
};

#endif // _WAPON_H_
