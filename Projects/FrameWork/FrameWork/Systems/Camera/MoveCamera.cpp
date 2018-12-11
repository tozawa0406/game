//-----------------------------------------------------------------------------
//
//	カメラ[Camera.cpp]
//	Auther : 戸澤翔太
//																	2017/11/07
//-----------------------------------------------------------------------------
#include "MoveCamera.h"
#include "../../Graphics/DirectX11/DirectX11.h"
#include "../../Scene/SceneManager.h"
#include "CameraManager.h"
#include "../GameSystems.h"
#include "../Input/KeyInput.h"

// コンストラクタ
MoveCamera::MoveCamera(CameraManager* parent, int number, VECTOR3 pos, VECTOR3 at) : Camera(parent, number, pos, at)
{
}

// デストラクタ
MoveCamera::~MoveCamera(void)
{
}

// 更新処理
void MoveCamera::Update(void)
{
	// 平行移動
	MoveTrans();

	// 自分を中心に回転
	MoveRotation();

	// 注視点を中心に回転
	MoveAtRotation();

	// 入力
	Input();
}

// 平行移動
void MoveCamera::MoveTrans(void)
{
	// 前ベクトルを算出
	front_ = at_ - pos_;
	front_.y = 0;
	front_ = VecNorm(front_);

	// 右ベクトルの求め方
	right_ = VecNorm(VecCross(front_, up_));

	// 速度が 0でない時
	if (velocity.x != 0 || velocity.y != 0)
	{
		// y座標移動
		pos_ += front_ * velocity.y;
		at_  += front_ * velocity.y;
		// x座標移動
		pos_ += right_ * velocity.x;
		at_  += right_ * velocity.x;

		// 慣性
		velocity *= 0.9f;
	}
}

// 自分を中心に回転
void MoveCamera::MoveRotation(void)
{
	// 速度が 0でない時
	if ( rotVelocity.x != 0 || rotVelocity.y != 0 )
	{
		// 回転行列
		MATRIX rotY, rotX;
		// y軸回転
		rotY.Identity().Rotation(VECTOR3(0, 1, 0), rotVelocity.y);
		// x軸回転
		rotX.Identity().Rotation(right_, rotVelocity.x);

		VECTOR3 vec = at_ - pos_;		// 方向ベクトル
		// ベクトルの座標変換
		vec = VecTransform(vec, rotY);
		vec = VecTransform(vec, rotX);

		// 単位ベクトル
		VECTOR3 check = vec;
		check = VecNorm(check);
		front_ = VecNorm(front_);

		// 内積による制限
		float limit = VecDot(front_, check);
		if (Abs(limit) >= 0.1f)
		{
			// 回転
			at_ = pos_ + vec;
		}
		else
		{
			// 制限なら速度を 0にする
			rotVelocity = { 0.0f, 0.0f }; 
		}

		// 慣性
		rotVelocity *= 0.9f;
	}
}

// 注視点を中心に回転
void MoveCamera::MoveAtRotation(void)
{
	// 速度が 0でない時
	if (rotAtVelocity.x != 0 || rotAtVelocity.y != 0)
	{
		// 回転行列
		MATRIX rotY, rotX;
		// y軸回転
		rotY.Identity().Rotation(VECTOR3(0, 1, 0), rotAtVelocity.x);
		// x軸回転
		rotX.Identity().Rotation(right_, rotAtVelocity.y);

		//ROTATION rot;		// 回転行列
		//D3DXMatrixRotationY(&rot.y, rotAtVelocity.x);				// y軸回転
		//D3DXMatrixRotationAxis(&rot.x, &D3DX(right), rotAtVelocity.y);	// 右ベクトル軸回転

		VECTOR3 vec = pos_ - at_;		// 方向ベクトル
		// ベクトルの座標変換	
		vec = VecTransform(vec, rotY);
		vec = VecTransform(vec, rotX);

		// 単位ベクトル
		VECTOR3 check = vec;
		check = VecNorm(check);
		front_ = VecNorm(front_);

		// 内積による制限
		float limit = VecDot(front_, check);
		if (Abs(limit) >= 0.1f)
		{
			// 回転
			pos_ = at_ + vec;
		}
		else
		{
			// 制限なら速度を 0にする
			rotAtVelocity = { 0.0f, 0.0f };
		}

		// 慣性
		rotAtVelocity *= 0.9f;
	}
}

// キー入力
void MoveCamera::Input(void)
{
	// 今自分がメインカメラでないなら操作不可
	if (parent_->GetMainNum() != number_) { return; }

	const auto& ctrl = systems_->GetInput()->GetCtrl(0);

	// 平行移動
	velocity.x -= static_cast<float>(ctrl->PressRange(Input::AXIS_LX, DIK_A, DIK_D)) * MOVE_SPEED;
	velocity.y += static_cast<float>(ctrl->PressRange(Input::AXIS_LY, DIK_S, DIK_W)) * MOVE_SPEED;

	// 自分を中心に回転
	if      (ctrl->Press(Input::GAMEPAD_LEFT , DIK_J)) { rotVelocity.y +=  ROTATION_SPEED; }
	else if (ctrl->Press(Input::GAMEPAD_RIGHT, DIK_L)) { rotVelocity.y += -ROTATION_SPEED; }
	if      (ctrl->Press(Input::GAMEPAD_UP   , DIK_I)) { rotVelocity.x += -ROTATION_SPEED; }
	else if (ctrl->Press(Input::GAMEPAD_DOWN , DIK_K)) { rotVelocity.x +=  ROTATION_SPEED; }

	// 注視点を中心に回転
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
