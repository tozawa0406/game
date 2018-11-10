/*
 * @file		PlayerMove.h
 * @brief		�v���C���[�̓���
 * @author		���V�đ�
 * @data		2018/11/02
 */
#ifndef _PLAYER_MOVE_H_
#define _PLAYER_MOVE_H_

#include <FrameWork/Systems/Camera/CameraManager.h>
#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

#include "../Wapon/Wapon.h"

struct MESH_ANIMATION
{
	MeshRenderer	mesh;				//! ���b�V��
	int				animation;			//! �A�j���[�V����
	float			animSpeed;			//! �A�j���[�V�����Đ����x
};

class PlayerState;
class PlayerMove : public Object, public GUI
{
protected:
	//! @def	�����t���O
	static constexpr uint IS_DRAWN			= 0x0001;
	//! @def	�[�������J�ڃt���O
	static constexpr uint IS_SETUP			= 0x0002;
	//! @def	������t���O
	static constexpr uint IS_AVOIDANCE		= 0x0004;
	//! @def	�U���t���O
	static constexpr uint IS_ATTACK			= 0x0008;
	//! @def	���̍U���t���O
	static constexpr uint IS_NEXT_SLASH		= 0x0010;
	//! @def	�A�j���[�V�����I���t���O
	static constexpr uint IS_END_ANIMATION	= 0x0020;


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
		Setup,
		SetupWait,
		SetupWalk,
		KnockBack,
		KnockOut,
		Slash_1,
		Slash_2,
		Slash_3,
		MAX
	};

	PlayerMove(void);
	virtual ~PlayerMove(void);

	virtual void Init(void)   override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	void GuiUpdate(void) override;

	/* @fn		SetWapon
	 * @brief	����̐ݒ�
	 * @param	(wapon)		����			*/
	inline void SetWapon(Wapon* wapon) { wapon_ = wapon; wapon->SetParent(transform_, body_, hand_); }

	inline MESH_ANIMATION&	GetMeshAnimation(void)	{ return meshAnim_;		}
	inline Camera*			GetCamera(void)			{ return camera_;		}
	inline const VECTOR3&	GetVelocity(void)		{ return velocity_;		}
	inline bool				IsEndAnim(void)			{ return isEndAnim_;	}
	inline Wapon*			GetWapon(void)			{ return wapon_;		}
	inline const VECTOR3&	GetFront(void)			{ return front_; }

	inline void SetVelocity(const VECTOR3& velocity) { velocity_ = velocity; }

protected:
	PlayerState*	state_;

	MESH_ANIMATION	meshAnim_;			//! ���b�V���ƃA�j���[�V�������
	VECTOR3			velocity_;			//! �ړ����x
	bool			isEndAnim_;			//! �A�j���[�V�����̏I������
	float			inputDash_;			//! �_�b�V������
	VECTOR2			inputDir_;			//! ���͕���
	VECTOR3			avoidanceDir_;		//! ������
	VECTOR3			front_;				//! �O�x�N�g��
	Wapon*			wapon_;				//! ����
	uint			flag_;				//! �t���O�̊Ǘ�
	CameraManager*	cameraManager_;		//! �J�����}�l�[�W���[
	Camera*			camera_;			//! �J����

	Collider3D::OBB* collider_;			//! �����蔻��


private:
	void Move(void);
	void CreateFrontVector(void);
	void OnGround(void);

	const MATRIX*	body_;				//! �̂̃{�[���s��
	const MATRIX*	hand_;				//! �E��̃{�[���s��
	int				waitTime_;			//! �ҋ@�҂�����
};

#endif // _PLAYER_MOVE_H_
