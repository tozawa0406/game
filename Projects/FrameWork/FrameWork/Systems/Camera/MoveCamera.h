//-----------------------------------------------------------------------------
//
//	�J����[Camera.h]
//	Auther : ���V�đ�
//																	2017/11/07
//-----------------------------------------------------------------------------
#ifndef _MOVE_CAMERA_H_
#define _MOVE_CAMERA_H_

#include "../../Define/Define.h"
#include "Camera.h"

class MoveCamera : public Camera
{
	friend class CameraManager;

	// �ړ����x
	static constexpr float MOVE_SPEED = 0.06f;
	// ��]���x
	static constexpr float ROTATION_SPEED = 0.002f;

public:
	// �f�X�g���N�^
	~MoveCamera(void);

	void Update(void);			// �X�V����
	void GuiUpdate(void);		// GUI

private:
	// �R���X�g���N�^
	MoveCamera(CameraManager* parent, int number, VECTOR3 pos = VECTOR3(Camera::POS_X, Camera::POS_Y, Camera::POS_Z),
		VECTOR3 at = VECTOR3(0.0f, 0.0f, 0.0f));
	void Input(void);			// �L�[����
	void MoveTrans(void);		// ���s�ړ�
	void MoveRotation(void);	// �����𒆐S�ɉ�]
	void MoveAtRotation(void);	// �����_�𒆐S�ɉ�]

	// ����
	VECTOR2 dir = { 0, 0 };
	// ���x
	VECTOR2 velocity = { 0, 0 };
	VECTOR2 rotVelocity = { 0, 0 };
	VECTOR2 rotAtVelocity = { 0, 0 };
};

#endif // _CAMERA_H_