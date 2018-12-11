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
	front_ = at_ - pos_;
	front_.y = 0;
	front_ = VecNorm(front_);

	// �E�x�N�g���̋��ߕ�
	right_ = VecNorm(VecCross(front_, up_));

	// ���x�� 0�łȂ���
	if (velocity.x != 0 || velocity.y != 0)
	{
		// y���W�ړ�
		pos_ += front_ * velocity.y;
		at_  += front_ * velocity.y;
		// x���W�ړ�
		pos_ += right_ * velocity.x;
		at_  += right_ * velocity.x;

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
		// ��]�s��
		MATRIX rotY, rotX;
		// y����]
		rotY.Identity().Rotation(VECTOR3(0, 1, 0), rotVelocity.y);
		// x����]
		rotX.Identity().Rotation(right_, rotVelocity.x);

		VECTOR3 vec = at_ - pos_;		// �����x�N�g��
		// �x�N�g���̍��W�ϊ�
		vec = VecTransform(vec, rotY);
		vec = VecTransform(vec, rotX);

		// �P�ʃx�N�g��
		VECTOR3 check = vec;
		check = VecNorm(check);
		front_ = VecNorm(front_);

		// ���ςɂ�鐧��
		float limit = VecDot(front_, check);
		if (Abs(limit) >= 0.1f)
		{
			// ��]
			at_ = pos_ + vec;
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
		// ��]�s��
		MATRIX rotY, rotX;
		// y����]
		rotY.Identity().Rotation(VECTOR3(0, 1, 0), rotAtVelocity.x);
		// x����]
		rotX.Identity().Rotation(right_, rotAtVelocity.y);

		//ROTATION rot;		// ��]�s��
		//D3DXMatrixRotationY(&rot.y, rotAtVelocity.x);				// y����]
		//D3DXMatrixRotationAxis(&rot.x, &D3DX(right), rotAtVelocity.y);	// �E�x�N�g������]

		VECTOR3 vec = pos_ - at_;		// �����x�N�g��
		// �x�N�g���̍��W�ϊ�	
		vec = VecTransform(vec, rotY);
		vec = VecTransform(vec, rotX);

		// �P�ʃx�N�g��
		VECTOR3 check = vec;
		check = VecNorm(check);
		front_ = VecNorm(front_);

		// ���ςɂ�鐧��
		float limit = VecDot(front_, check);
		if (Abs(limit) >= 0.1f)
		{
			// ��]
			pos_ = at_ + vec;
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
	if (parent_->GetMainNum() != number_) { return; }

	const auto& ctrl = systems_->GetInput()->GetCtrl(0);

	// ���s�ړ�
	velocity.x -= static_cast<float>(ctrl->PressRange(Input::AXIS_LX, DIK_A, DIK_D)) * MOVE_SPEED;
	velocity.y += static_cast<float>(ctrl->PressRange(Input::AXIS_LY, DIK_S, DIK_W)) * MOVE_SPEED;

	// �����𒆐S�ɉ�]
	if      (ctrl->Press(Input::GAMEPAD_LEFT , DIK_J)) { rotVelocity.y +=  ROTATION_SPEED; }
	else if (ctrl->Press(Input::GAMEPAD_RIGHT, DIK_L)) { rotVelocity.y += -ROTATION_SPEED; }
	if      (ctrl->Press(Input::GAMEPAD_UP   , DIK_I)) { rotVelocity.x += -ROTATION_SPEED; }
	else if (ctrl->Press(Input::GAMEPAD_DOWN , DIK_K)) { rotVelocity.x +=  ROTATION_SPEED; }

	// �����_�𒆐S�ɉ�]
	rotAtVelocity.x -= static_cast<float>(ctrl->PressRange(Input::AXIS_RX, DIK_Q, DIK_E)) * ROTATION_SPEED;
	rotAtVelocity.y -= static_cast<float>(ctrl->PressRange(Input::AXIS_RY, DIK_G, DIK_T)) * ROTATION_SPEED;
}

// GUI
void MoveCamera::GuiUpdate(void)
{
	ImGui::Text("pos x:y:z  %.2f:%.2f:%.2f", pos_.x, pos_.y, pos_.z);
	ImGui::Text("at  x:y:z  %.2f:%.2f:%.2f", at_.x, at_.y, at_.z);
	if (parent_->GetMainNum() < 0)
	{
		ImGui::Text("main debug");
	}
	else
	{
		ImGui::Text("main %d", parent_->GetMainNum());
	}
}
