//-----------------------------------------------------------------------------
//
//	�I�u�W�F�N�g�Ǐ]�J����[AtObjCamera.h]
//	Auther : ���V�đ�
//																	2018/01/24
//-----------------------------------------------------------------------------
#ifndef _AT_OBJ_CAMERA_H_
#define _AT_OBJ_CAMERA_H_

#include "../../Define/Define.h"
#include "Camera.h"

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class MobileSuits;
class AtObjCamera : public Camera
{
	friend class CameraManager;

	static constexpr float ROTATION_SPEED				= -0.00175f;
	static constexpr float MAX_ROTATION_SPEED			= 0.05f;
	static constexpr float ROTATION_AXIS_COEFFICIENT	= 1.0f / 25.0f;

public:	
	// �f�X�g���N�^
	~AtObjCamera(void);

	// �X�V����
	void Update(void);
	// ImGui����(�f�o�b�O���̂�)
	void GuiUpdate(void);

	void DestroyObject(void) { obj_ = nullptr; }

private:
	// �R���X�g���N�^
	AtObjCamera(CameraManager* parent, int number, VECTOR3 pos, Object* obj);
	// �x�N�g����������
	void CreateVec(void);
	// ���͏���
	void Input(void);
	// �s�񏈗�
	void CreateMtx(void);
	// �ړ�����
	void Move(void);

	// �Ǐ]�I�u�W�F�N�g
	Object* obj_;

	VECTOR3 localPosition_;
	// ��](�W�I�����Ȃ��Ƃ��Ɏg�p)
	VECTOR2 rot_;
};

#endif // _AT_OBJ_CAMERA_H_