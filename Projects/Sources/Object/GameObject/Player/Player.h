/*
 * @file		Player.h
 * @brief		�v���C���[�̓���
 * @author		���V�đ�
 * @data		2018/11/02
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <FrameWork/Systems/Camera/CameraManager.h>
#include "../GameObject.h"
#include "../../Wapon/Wapon.h"

class PlayerState;
class Player : public GameObject, public GUI
{
protected:
	//! @def	�A�j���[�V�����̑��x
	static constexpr float ANIMATION_DEFAULT = 0.75f;

public:
	//! @enum	�A�j���[�V����
	enum class Animation
	{
		Wait = 0,
		WaitTime1,
		WaitTime2,
		Walk,
		Run,
		Roll,
		Dive,
		KnockBack,
		KnockOut,
		Die,
		Standup,
		WalkSetup,
		Setup,
		SetupDrawn,
		SetupWait,
		SetupWalk,
		Slash_1,
		Slash_2,
		Slash_3,
		MAX
	};
	//! @def	�X�^�~�i�񕜒l
	static constexpr float ADD_STAMINA = 0.2f;

	/* @brief	�R���X�g���N�^		*/
	Player(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~Player(void);


	
	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	virtual void Init(void)   override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	virtual void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	virtual void Update(void) override;
	
	/* @brief	�f�o�b�O�p�`��X�V
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void GuiUpdate(void) override;
	
	/* @brief	�_���[�W����
	 * @param	(damage)	�_���[�W
	 * @return	�Ȃ�				*/
	void Hit(int damage) override;

	/* @brief	����̐ݒ�
	 * @param	(wapon)		����			*/
	inline void SetWapon(Wapon* wapon) { wapon_ = wapon; wapon->SetParent(transform_, body_, hand_); }

	/* @brief	�J�����擾����				*/
	inline Camera*	GetCamera(void)	{ return camera_;		}
	/* @brief	����̎擾����				*/
	inline Wapon*	GetWapon(void)	{ return wapon_;		}

	/* @brief	�X�e�[�g�Őݒ�				*/
	inline void SetPosition(const VECTOR3& position) { transform_.position = position; }

	/* @brief	�����蔻��̎擾����		*/
	inline Collider3D::OBB* GetCollider(void) { return collider_; }

	/* @brief	�f�o�b�O�p�F�G�̑��쒆�Ƀv���C���[�𑀍삵�Ȃ�	*/
	inline bool IsDebugCtrl(void) { return (cameraManager_ && cameraManager_->GetMainNum() != 0) ? true : false; }

	/* @brief	�[�������̕ύX				*/
	inline void SetDraw(bool isDraw) { isDraw_ = isDraw;	}
	/* @brief	�[�������̎擾				*/
	inline bool IsDraw(void)		 { return isDraw_;		}

	/* @brief	�X�^�~�i�̎擾				*/
	inline float GetStamina(void)			{ return stamina_;		}
	/* @brief	�X�^�~�i�̐ݒ�				*/
	inline void  SetStamina(float stamina)	{ stamina_ = stamina;	}

private:
	/* @brief	���S����
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	����ł�����true			*/
	bool IsDedJudge(void);

	PlayerState*	state_;				//! �v���C���[�̃X�e�[�g
	float			stamina_;			//! �X�^�~�i
	bool			isDraw_;			//! �����t���O

	Wapon*			wapon_;				//! ����
	CameraManager*	cameraManager_;		//! �J�����}�l�[�W���[
	Camera*			camera_;			//! �J����

	Collider3D::OBB* collider_;			//! �����蔻��

	const MATRIX*	body_;				//! �̂̃{�[���s��
	const MATRIX*	hand_;				//! �E��̃{�[���s��
};

#endif // _PLAYER_H_
