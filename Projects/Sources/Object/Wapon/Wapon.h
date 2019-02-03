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

#include "../GameObject/AttackManager.h"

class Wapon : public Object
{
public:
	Wapon(void) : Object(ObjectTag::WAPON), collider_(nullptr), body_(nullptr), hand_(nullptr), effectRotation_(0), offensivePower_(0), baseOffensivePower_(0) {}
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
	inline void AttackStart(void) 
	{
		if (collider_) { collider_->SetEnable(true); } 

		if (!attackManager_) { return; }
		attackID_ = attackManager_->CreateAttackID();
	}

	/* @brief	�U���I������				*/
	inline void AttackEnd(void)   { if (collider_) { collider_->SetEnable(false); } }

	/* @brief	�U������					*/
	inline bool IsAttack(void) { return collider_->IsEnable(); }

	/* @brief	�U���Ǘ��N���X�̐ݒ�		*/
	inline void SetAttackManager(AttackManager* attackManager) { attackManager_ = attackManager; }

	inline void SetOffensivePower(int offensivePower) { offensivePower_ = offensivePower + baseOffensivePower_; }

protected:
	//! �[������
	virtual void PaidSword(void) = 0;
	//! ��������
	virtual void DrawnSword(void) = 0;

	//! �U���Ǘ��N���X
	AttackManager* attackManager_;
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
	//! �U����
	int offensivePower_;
	//! ���했�̊�b�U����
	int baseOffensivePower_;

	//! �O��̍U��ID
	uint8 attackID_;
};

#endif // _WAPON_H_
