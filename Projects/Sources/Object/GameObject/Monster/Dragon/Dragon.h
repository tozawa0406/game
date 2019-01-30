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

class Dragon : public GameObject, public GUI
{
	//! @enum	�����蔻��
	enum class Collision : uint8
	{
		BODY = 0,
		HEAD,
		NECK1,
		NECK2,
		TAIL,
		WING_R,
		WING_L,
		MAX
	};

	//! @enum	�U���p�^�[��
	enum class AttackPattern : uint8
	{
		SCREAM = 0,
		BITE,
		WING_ATTACK,
		TAIL_ATTACK,
		TAKE_OFF,
		HIT,
		RUSH,

		MAX
	};
public:
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
	 * @return	�Ȃ�				*/
	void Hit(int damage) override;

private:
	/* @brief	�����蔻�萶������
	 * @sa		Init
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void CreateCollision(void);
	
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
	void SetCollision(int arrayNum, string boneName, const Transform& offset, const MODEL& model);

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
};

#endif // _DRAGON_H_
