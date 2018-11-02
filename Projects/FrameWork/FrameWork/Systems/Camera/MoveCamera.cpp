//-----------------------------------------------------------------------------
//
//	�J����[Camera.cpp]
//	Auther : ���V�đ�
//																	2017/11/07
//-----------------------------------------------------------------------------
#include "MoveCamera.h"
#include "../../Graphics/DirectX11/DirectX11.h"
#include "../../Scene/SceneManager.h"
#include "CameraManager.h"
#include "../GameSystems.h"
#include "../Input/KeyInput.h"

// �R���X�g���N�^
MoveCamera::MoveCamera(CameraManager* parent, int number, VECTOR3 pos, VECTOR3 at) : Camera(parent, number, pos, at)
{
}

// �f�X�g���N�^
MoveCamera::~MoveCamera(void)
{
}

// �X�V����
void MoveCamera::Update(void)
{
	// ���s�ړ�
	MoveTrans();

	// �����𒆐S�ɉ�]
	MoveRotation();

	// �����_�𒆐S�ɉ�]
	MoveAtRotation();

	// ����
	Input();
}

// ���s�ړ�
void MoveCamera::MoveTrans(void)
{
	// �O�x�N�g�����Z�o
	front = at - pos;
	front.y = 0;
	front = VecNorm(front);

	// �E�x�N�g���̋��ߕ�
	right = VecNorm(VecCross(front, up));

	// ���x�� 0�łȂ���
	if (velocity.x != 0 || velocity.y != 0)
	{
		// y���W�ړ�
		pos += front * velocity.y;
		at  += front * velocity.y;
		// x���W�ړ�
		pos += right * velocity.x;
		at  += right * velocity.x;

		// ����
		velocity *= 0.9f;
	}
}

// �����𒆐S�ɉ�]
void MoveCamera::MoveRotation(void)
{
	// ���x�� 0�łȂ���
	if ( rotVelocity.x != 0 || rotVelocity.y != 0 )
	{
		ROTATION rot;		//�@��]�s��
		D3DXMatrixRotationY(&rot.y, rotVelocity.y);				// y����]
		D3DXMatrixRotationAxis(&rot.x, &D3DX(right), rotVelocity.x);	// �E�x�N�g������]

		VECTOR3 vec = at - pos;		// �����x�N�g��
		// �x�N�g���̍��W�ϊ�
		D3DXVECTOR3 out = D3DX(vec);
		D3DXVec3TransformNormal(&out, &out, &rot.y);
		D3DXVec3TransformNormal(&out, &out, &rot.x);
		vec = V(out);

		// �P�ʃx�N�g��
		VECTOR3 check = vec;
		check = VecNorm(check);
		front = VecNorm(front);

		// ���ςɂ�鐧��
		float limit = VecDot(front, check);
		if (Abs(limit) >= 0.1f)
		{
			// ��]
			at = pos + vec;
		}
		else
		{
			// �����Ȃ瑬�x�� 0�ɂ���
			rotVelocity = { 0.0f, 0.0f }; 
		}

		// ����
		rotVelocity *= 0.9f;
	}
}

// �����_�𒆐S�ɉ�]
void MoveCamera::MoveAtRotation(void)
{
	// ���x�� 0�łȂ���
	if (rotAtVelocity.x != 0 || rotAtVelocity.y != 0)
	{
		ROTATION rot;		// ��]�s��
		D3DXMatrixRotationY(&rot.y, rotAtVelocity.x);				// y����]
		D3DXMatrixRotationAxis(&rot.x, &D3DX(right), rotAtVelocity.y);	// �E�x�N�g������]

		VECTOR3 vec = pos - at;		// �����x�N�g��
		// �x�N�g���̍��W�ϊ�	
		D3DXVECTOR3 out = D3DX(vec);
		D3DXVec3TransformNormal(&out, &out, &rot.y);
		D3DXVec3TransformNormal(&out, &out, &rot.x);
		vec = V(out);


		// �P�ʃx�N�g��
		VECTOR3 check = vec;
		check = VecNorm(check);
		front = VecNorm(front);

		// ���ςɂ�鐧��
		float limit = VecDot(front, check);
		if (Abs(limit) >= 0.1f)
		{
			// ��]
			pos = at + vec;
		}
		else
		{
			// �����Ȃ瑬�x�� 0�ɂ���
			rotAtVelocity = { 0.0f, 0.0f };
		}

		// ����
		rotAtVelocity *= 0.9f;
	}
}

// �L�[����
void MoveCamera::Input(void)
{
	// �����������C���J�����łȂ��Ȃ瑀��s��
	int a = parent->GetMainNum();
	if (parent->GetMainNum() != this->number) { return; }

	// ���s�ړ�
	if      (systems->GetInput()->GetKeyboard()->Press(DIK_W)) { velocity.y +=  MOVE_SPEED; }
	else if (systems->GetInput()->GetKeyboard()->Press(DIK_S)) { velocity.y += -MOVE_SPEED; }
	if      (systems->GetInput()->GetKeyboard()->Press(DIK_A)) { velocity.x +=  MOVE_SPEED; }
	else if (systems->GetInput()->GetKeyboard()->Press(DIK_D)) { velocity.x += -MOVE_SPEED; }

	// �����𒆐S�ɉ�]
	if      (systems->GetInput()->GetKeyboard()->Press(DIK_H)) { rotVelocity.y += -ROTATION_SPEED; }
	else if (systems->GetInput()->GetKeyboard()->Press(DIK_K)) { rotVelocity.y +=  ROTATION_SPEED; }
	if      (systems->GetInput()->GetKeyboard()->Press(DIK_U)) { rotVelocity.x +=  ROTATION_SPEED; }
	else if (systems->GetInput()->GetKeyboard()->Press(DIK_J)) { rotVelocity.x += -ROTATION_SPEED; }

	// �����_�𒆐S�ɉ�]
	if      (systems->GetInput()->GetKeyboard()->Press(DIK_Q)) { rotAtVelocity.x +=  ROTATION_SPEED; }
	else if (systems->GetInput()->GetKeyboard()->Press(DIK_E)) { rotAtVelocity.x += -ROTATION_SPEED; }
	if      (systems->GetInput()->GetKeyboard()->Press(DIK_T)) { rotAtVelocity.y += -ROTATION_SPEED; }
	else if (systems->GetInput()->GetKeyboard()->Press(DIK_G)) { rotAtVelocity.y +=  ROTATION_SPEED; }
}

// GUI
void MoveCamera::GuiUpdate(void)
{
	ImGui::Text("pos x:y:z  %.2f:%.2f:%.2f", pos.x, pos.y, pos.z);
	ImGui::Text("at  x:y:z  %.2f:%.2f:%.2f", at.x, at.y, at.z);
	if (parent->GetMainNum() < 0)
	{
		ImGui::Text("main debug");
	}
	else
	{
		ImGui::Text("main %d", parent->GetMainNum());
	}
}
