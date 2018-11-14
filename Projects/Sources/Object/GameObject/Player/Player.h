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
		KnockBack,
		KnockOut,
		Die,
		Standup,
		Setup,
		SetupWait,
		SetupWalk,
		Slash_1,
		Slash_2,
		Slash_3,
		MAX
	};

	Player(void);
	virtual ~Player(void);

	virtual void Init(void)   override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	void GuiUpdate(void) override;

	void Hit(int damage) override;

	/* @fn		SetWapon
	 * @brief	����̐ݒ�
	 * @param	(wapon)		����			*/
	inline void SetWapon(Wapon* wapon) { wapon_ = wapon; wapon->SetParent(transform_, body_, hand_); }

	// �X�e�[�g���ɒl���Q�Ƃ��邽�߂�Getter
	inline Camera*			GetCamera(void)			{ return camera_;		}
	inline Wapon*			GetWapon(void)			{ return wapon_;		}

	// �X�e�[�g�Œl��ݒ肷�邽�߂�Setter
	inline void SetPosition(const VECTOR3& position) { transform_.position = position; }

	inline Collider3D::OBB* GetCollider(void) { return collider_; }

	// �f�o�b�O�p�A�G�̑��쒆�Ƀv���C���[�𑀍삵�Ȃ�
	inline bool IsDebugCtrl(void) { return (cameraManager_ && cameraManager_->GetMainNum() != 0) ? true : false; }

private:
	bool IsDed(void);

	PlayerState*	state_;				//! �v���C���[�̃X�e�[�g

	Wapon*			wapon_;				//! ����
	CameraManager*	cameraManager_;		//! �J�����}�l�[�W���[
	Camera*			camera_;			//! �J����

	Collider3D::OBB* collider_;			//! �����蔻��

	const MATRIX*	body_;				//! �̂̃{�[���s��
	const MATRIX*	hand_;				//! �E��̃{�[���s��
};

#endif // _PLAYER_H_
