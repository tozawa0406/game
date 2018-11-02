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
	front = at - pos;
	front.y = 0;
	front = VecNorm(front);

	// 右ベクトルの求め方
	right = VecNorm(VecCross(front, up));

	// 速度が 0でない時
	if (velocity.x != 0 || velocity.y != 0)
	{
		// y座標移動
		pos += front * velocity.y;
		at  += front * velocity.y;
		// x座標移動
		pos += right * velocity.x;
		at  += right * velocity.x;

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
		ROTATION rot;		//　回転行列
		D3DXMatrixRotationY(&rot.y, rotVelocity.y);				// y軸回転
		D3DXMatrixRotationAxis(&rot.x, &D3DX(right), rotVelocity.x);	// 右ベクトル軸回転

		VECTOR3 vec = at - pos;		// 方向ベクトル
		// ベクトルの座標変換
		D3DXVECTOR3 out = D3DX(vec);
		D3DXVec3TransformNormal(&out, &out, &rot.y);
		D3DXVec3TransformNormal(&out, &out, &rot.x);
		vec = V(out);

		// 単位ベクトル
		VECTOR3 check = vec;
		check = VecNorm(check);
		front = VecNorm(front);

		// 内積による制限
		float limit = VecDot(front, check);
		if (Abs(limit) >= 0.1f)
		{
			// 回転
			at = pos + vec;
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
		ROTATION rot;		// 回転行列
		D3DXMatrixRotationY(&rot.y, rotAtVelocity.x);				// y軸回転
		D3DXMatrixRotationAxis(&rot.x, &D3DX(right), rotAtVelocity.y);	// 右ベクトル軸回転

		VECTOR3 vec = pos - at;		// 方向ベクトル
		// ベクトルの座標変換	
		D3DXVECTOR3 out = D3DX(vec);
		D3DXVec3TransformNormal(&out, &out, &rot.y);
		D3DXVec3TransformNormal(&out, &out, &rot.x);
		vec = V(out);


		// 単位ベクトル
		VECTOR3 check = vec;
		check = VecNorm(check);
		front = VecNorm(front);

		// 内積による制限
		float limit = VecDot(front, check);
		if (Abs(limit) >= 0.1f)
		{
			// 回転
			pos = at + vec;
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
	int a = parent->GetMainNum();
	if (parent->GetMainNum() != this->number) { return; }

	// 平行移動
	if      (systems->GetInput()->GetKeyboard()->Press(DIK_W)) { velocity.y +=  MOVE_SPEED; }
	else if (systems->GetInput()->GetKeyboard()->Press(DIK_S)) { velocity.y += -MOVE_SPEED; }
	if      (systems->GetInput()->GetKeyboard()->Press(DIK_A)) { velocity.x +=  MOVE_SPEED; }
	else if (systems->GetInput()->GetKeyboard()->Press(DIK_D)) { velocity.x += -MOVE_SPEED; }

	// 自分を中心に回転
	if      (systems->GetInput()->GetKeyboard()->Press(DIK_H)) { rotVelocity.y += -ROTATION_SPEED; }
	else if (systems->GetInput()->GetKeyboard()->Press(DIK_K)) { rotVelocity.y +=  ROTATION_SPEED; }
	if      (systems->GetInput()->GetKeyboard()->Press(DIK_U)) { rotVelocity.x +=  ROTATION_SPEED; }
	else if (systems->GetInput()->GetKeyboard()->Press(DIK_J)) { rotVelocity.x += -ROTATION_SPEED; }

	// 注視点を中心に回転
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
