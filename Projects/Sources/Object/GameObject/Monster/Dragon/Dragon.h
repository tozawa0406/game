/*
 * @file		Dragon.h
 * @brief		�G�̓���
 * @author		���V�đ�
 * @data		2018/11/02
 */
#ifndef _DRAGON_H_
#define _DRAGON_H_

#include <FrameWork/Systems/Camera/CameraManager.h>
#include "../../GameObject.h"
#include "../MonsterAttack.h"
#include "DragonMoveController.h"

struct BONE_COLLISION
{
	string	boneName;
	VECTOR3 offsetPosition;
	VECTOR3 offsetRotation;
	VECTOR3 size;
};

class Dragon : public GameObject, public GUI
{
	//! @enum	�U���p�^�[��
	enum class AttackPattern : uint8
	{
		SCREAM = 0,
		BITE,
		WING_ATTACK,
		TAIL_ATTACK,
		RUSH,
		TAKE_OFF,
		HIT,

		MAX
	};
public:
	//! @enum	�����蔻��
	enum class Collision : uint8
	{
		BODY = 0,
		HEAD,
		NECK1,
		NECK2,
		WING_LL,
		WING_LC,
		WING_LR,
		WING_L_CLAW,
		WING_RL,
		WING_RC,
		WING_RR,
		WING_R_CLAW,
		ARM_L_UP,
		ARM_L_UP2,
		ARM_L_DOWN,
		ARM_R_UP,
		ARM_R_UP2,
		ARM_R_DOWN,
		LEGS_L_UP,
		LEGS_L_UP2,
		LEGS_L_DOWN,
		LEGS_R_UP,
		LEGS_R_UP2,
		LEGS_R_DOWN,
		TAIL5,
		TAIL4,
		TAIL3,
		TAIL2,
		TAIL1,
		MAX
	};

	//! @enum	�A�j���[�V����
	enum class Animation : uint8
	{
		WAIT1 = 0,
		WAIT2,
		WALK,
		RUN,
		SCREAM,
		HIT,
		DIE,
		BITE,
		WING_ATTACK,
		TAIL_ATTACK,
		TAKE_OFF,
		FLY_IDLE,

		MAX
	};
	//! @def	�ړ����x
	static constexpr float MOVE_SPEED = 0.03f;
	//! @def	����t���O
	static constexpr uint IS_DASH = 0x0001;

	/* @brief	�R���X�g���N�^		*/
	Dragon(void);
	/* @brief	�f�X�g���N�^		*/
	~Dragon(void);


	
	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void)	  override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Update(void) override;
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void GuiUpdate(void) override;
	
	/* @brief	�_���[�W����
	 * @param	(damage)	�_���[�W
	 * @param	(attackID)	�U��ID
	 * @return	�Ȃ�				*/
	void Hit(int damage, uint8 attackID) override;

	/* @brief	�ʒu�ݒ�
	 * @param	(pos)	�ݒ肵�����ʒu
	 * @return	�Ȃ�				*/
	inline void SetPosition(const VECTOR3& pos) { transform_.position = pos; }

	/* @brief	��]�ݒ�
	 * @param	(rot)	�ݒ肵������]
	 * @return	�Ȃ�				*/
	inline void SetRotation(const VECTOR3& rot) { transform_.rotation = rot; }

	/* @breif	�R���C�_�[���擾
	 * @param	(num)	�R���C�_�[�ԍ�
	 * @return	�R���C�_�[���		*/
	const Collider3D::OBB* GetCollider(Collision num) const { return collision_[static_cast<int>(num)]; }

	/* @brief	�����蔻�薳�����X�g�̒ǉ�
	 * @param	(tag)	�ǉ�����^�O
	 * @return	�Ȃ�				*/
	inline void AddIgnore(ObjectTag addTag) { for (auto& c : collision_) { c->AddIgnoreList(addTag); } }

	/* @brief	�����蔻��̖������X�g�̍폜
	 * @param	(tag)	�폜����^�O
	 * @return	�Ȃ�				*/
	inline void RemoveIgnore(ObjectTag removeTag) { for (auto& c : collision_) { c->RemoveIgnoreList(removeTag); } }

private:
	/* @brief	�����蔻�萶������
	 * @sa		Init
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void CreateCollision(void);

	template<class T>
	void CreateAttack(AttackPattern attack);

	/* @brief	�f�o�b�O�p����
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	���s������true		*/
	bool DebugInput(void);
	
	/* @brief	��_���[�W����
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	bool TakenDamage(void);
	
	/* @brief	�R���W�����̐ݒ�
	 * @param	(arrayNum)	��������R���W�����̔z��ԍ�
	 * @param	(boneName)	�{�[���̖��O
	 * @param	(offset)	�I�t�Z�b�g
	 * @param	(model)		���f���f�[�^
	 * @return	�Ȃ�				*/
	void SetCollision(int arrayNum, const BONE_COLLISION& offset, const MODEL& model);

	//! �t���O
	uint				flag_;
	//! �����蔻��
	Collider3D::OBB*	collision_[static_cast<uint8>(Collision::MAX)];
	Collider3D::OBB*	collisionDefense_[static_cast<uint8>(Collision::MAX)];

	//! �U�������̃C���X�^���X
	MonsterAttack*			attack_[static_cast<int>(AttackPattern::MAX)];
	//! ���݂̍U������
	MonsterAttack*			currentAttack_;
	//! �U����������
	DragonMoveController*	moveController_;

	int				accumulation_;		//! ��_���[�W�~�ϒl

	CameraManager*	cameraManager_;		//! �J�����}�l�[�W���[
	Camera*			camera_;			//! �J����

	bool			debugMove_;			//! �f�o�b�O�p�R���g���[���t���O

	static const BONE_COLLISION BONE_COLLISION_OFFSET[static_cast<int>(Collision::MAX)];
};

#endif // _DRAGON_H_
