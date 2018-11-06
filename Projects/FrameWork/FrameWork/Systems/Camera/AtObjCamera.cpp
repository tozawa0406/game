//-----------------------------------------------------------------------------
//
//	�I�u�W�F�N�g�Ǐ]�J����[AtObjCamera.cpp]
//	Auther : ���V�đ�
//																	2018/01/24
//-----------------------------------------------------------------------------
#include "AtObjCamera.h"
#include "../../Scene/SceneManager.h"
#include "CameraManager.h"

// �R���X�g���N�^
AtObjCamera::AtObjCamera(CameraManager* parent, int number, VECTOR3 pos, Object* obj) : Camera(parent, number, pos, obj->GetTransform().position)
											, obj_(obj), rot_(VECTOR2(0, 0)), localPosition_(pos)
{
}

// �f�X�g���N�^
AtObjCamera::~AtObjCamera(void)
{
}

// �X�V����
void AtObjCamera::Update(void)
{
	// �x�N�g������
	this->CreateVec();

	// ���͏���
	this->Input();

	// �s�񐶐�
	this->CreateMtx();

	// �ړ�����
	this->Move();
}

// ImGui����
void AtObjCamera::GuiUpdate(void)
{
	ImGui::Text("front x:y:z %.2f:%.2f:%.2f", front_.x, front_.y, front_.z);
	ImGui::Text("pos x:y:z  %.2f:%.2f:%.2f", pos_.x, pos_.y, pos_.z);
	ImGui::Text("at  x:y:z  %.2f:%.2f:%.2f", at_.x, at_.y, at_.z);
	ImGui::Text("rot x:y  %.2f:%.2f", rot_.x, rot_.y);
	if (parent_->GetMainNum() < 0)
	{
		ImGui::Text("main debug");
	}
	else
	{
		ImGui::Text("main %d", parent_->GetMainNum());
	}
}

// �x�N�g����������
void AtObjCamera::CreateVec(void)
{
	if (obj_ == nullptr) { return; }

	// �O�x�N�g���͒Ǐ]�I�u�W�F�N�g�Ǝ����̋���
	front_ = obj_->GetTransform().position - pos_;
	// �O�x�N�g���̐��K��
	front_ = VecNorm(front_);
	frontXPlane_ = VecNorm(VECTOR3(front_.x, 0, front_.z));
	at_ = obj_->GetTransform().position + (VecNorm(frontXPlane_) * 10) + (up_ * 15);
}

// ���͏���
void AtObjCamera::Input(void)
{
	if (parent_ && parent_->GetMainNum() != number_) { return; }
	const auto& ctrl = systems_->GetInput()->GetCtrl(0);

	// ���͂����m
	VECINT2 rAxis;
	if (ctrl->GetCtrlNum() == Controller::CtrlNum::Key)
	{
		rAxis.x = ctrl->PressRange(Input::AXIS_RX, DIK_J, DIK_L) * 2;
		rAxis.y = ctrl->PressRange(Input::AXIS_RY, DIK_I, DIK_K) * 2;
	}
	else
	{
		const auto& axis = ctrl->GetAxis();
		rAxis.x = (int)(axis.stickRX * ROTATION_AXIS_COEFFICIENT);
		rAxis.y = (int)(axis.stickRY * ROTATION_AXIS_COEFFICIENT);
	}

	// ����]
	rot_.x += ((ROTATION_SPEED * 2) * rAxis.x * fabs((float)rAxis.x));
	// �c��]
	rot_.y += -(ROTATION_SPEED      * rAxis.y * fabs((float)rAxis.y));

	// ����
	rot_ *= 0.7f;

	rot_.x = max(min(rot_.x, MAX_ROTATION_SPEED), -MAX_ROTATION_SPEED);
	rot_.y = max(min(rot_.y, MAX_ROTATION_SPEED), -MAX_ROTATION_SPEED);
}

// �s�񏈗�
void AtObjCamera::CreateMtx(void)
{
	if (obj_ == nullptr) { return; }

	// �v���C���[�s��̐���
	VECTOR3 moveBase = obj_->GetTransform().position;
	moveBase.y += 10;
	MATRIX mtxObj;
	mtxObj.Identity().Translation(moveBase);

	// �ړ�
	MATRIX mtx;
	mtx.Identity().Translation(localPosition_);
	// �E�x�N�g���̐���
	right_ = VecCross(front_, up_);
	// �E�x�N�g���̐��K��
	right_ = VecNorm(right_);

	rightXPlane_ = VecNorm(VECTOR3(right_.x, 0, right_.z));

	// �v���C���[�̈ʒu������ɉ�����
	mtx *= mtxObj;

	pos_ = VECTOR3(mtx._41, mtx._42, mtx._43);
}

// �ړ�����
void AtObjCamera::Move(void)
{
	// ���x�� 0�łȂ���
	if (fabs(rot_.x) > 0.0001f || fabs(rot_.y) > 0.0001f)
	{
		MATRIX mtxRotX, mtxRotY = MATRIX().Identity();
		// ����]
		mtxRotX.Identity().Rotation(VECTOR3(0, rot_.x, 0));
		// �c��](������t���Ȃ��Ɛ^��^���ɗ���(���x�������Ȃ��Ɩ߂�Ȃ��Ȃ�))
		if (((front_.y < 0.9f) && rot_.y < 0) || ((front_.y > -0.9f) && rot_.y > 0))
		{
			mtxRotY.Rotation(right_, rot_.y);
		}
		else { rot_.y = 0; }
		
		// �x�N�g���̍��W�ϊ�
		localPosition_ = VecTransformNormal(localPosition_, mtxRotX);
		localPosition_ = VecTransformNormal(localPosition_, mtxRotY);
	}
	else
	{
		rot_ = VECTOR2(0, 0);
	}
}
