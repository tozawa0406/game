//-----------------------------------------------------------------------------
//
//	�J����[Camera.h]
//	Auther : ���V�đ�
//																	2017/11/07
//-----------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../../Define/Define.h"
#include "../DebugSetting/GUI.h"

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class Object;
class Systems;
class Camera : public GUI
{
	friend class CameraManager;
public:
	// ��p
	static constexpr int FOV = 80;
	// �j�A�N���b�v
	static constexpr float C_NEAR = 0.1f;
	// �t�@�[�N���b�v
	static constexpr float C_FAR = 1000.0f;

	// �J�����̏������W
	static constexpr float POS_X = 0.0f;
	static constexpr float POS_Y = 10.0f;
	static constexpr float POS_Z = -50.0f;

	// �f�X�g���N�^
	virtual ~Camera(void) {};

	virtual void Update(void) {};			// �X�V����

	// Getter
	const VECTOR3& GetPos(void) { return pos_; }
	const VECTOR3& GetAt(void)  { return at_;  }
	const VECTOR3& GetUp(void)  { return up_;  }

	const VECTOR3& GetFront(void) { return front_; }
	const VECTOR3& GetRight(void) { return right_; }

	const VECTOR3& GetFrontXPlane(void) { return frontXPlane_; }
	const VECTOR3& GetRightXPlane(void) { return rightXPlane_; }

	// Setter
	void SetPos(VECTOR3 pos) { pos_ = pos; }
	void SetAt(VECTOR3 at)   { at_ = at; }

	void SetCamera(Camera* camera)
	{
		pos_ = camera->pos_;
		at_  = camera->at_;
	}

protected:
	// �R���X�g���N�^
	Camera(CameraManager* parent, int number, VECTOR3 pos = VECTOR3(POS_X, POS_Y, POS_Z),
		VECTOR3 at = VECTOR3(0.0f, 0.0f, 0.0f));

	// �e�N���X
	CameraManager* parent_;

	int     number_;
	VECTOR3 pos_;		// �J�����̈ʒu���W
	VECTOR3 at_;		// �����_���W
	VECTOR3 up_;		// �J�����̏��\��
	VECTOR3 front_;		// �O�x�N�g��
	VECTOR3 right_;		// �E�x�N�g��

	VECTOR3 frontXPlane_;	//! �O�x�N�g�����獂������(y)���������l
	VECTOR3 rightXPlane_;	//! �O�x�N�g�����獂������(y)���������l

	Systems* systems_;
};

#endif // _CAMERA_H_