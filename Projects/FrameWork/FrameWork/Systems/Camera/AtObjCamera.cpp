//-----------------------------------------------------------------------------
//
//	オブジェクト追従カメラ[AtObjCamera.cpp]
//	Auther : 戸澤翔太
//																	2018/01/24
//-----------------------------------------------------------------------------
#include "AtObjCamera.h"
#include "../../Scene/SceneManager.h"
#include "CameraManager.h"

// コンストラクタ
AtObjCamera::AtObjCamera(CameraManager* parent, int number, VECTOR3 pos, Object* obj) : Camera(parent, number, pos, obj->GetTransform().position)
											, obj_(obj), rot_(VECTOR2(0, 0)), localPosition_(pos)
{
}

// デストラクタ
AtObjCamera::~AtObjCamera(void)
{
}

// 更新処理
void AtObjCamera::Update(void)
{
	// ベクトル生成
	this->CreateVec();

	// 入力処理
	this->Input();

	// 行列生成
	this->CreateMtx();

	// 移動処理
	this->Move();
}

// ImGui処理
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

// ベクトル生成処理
void AtObjCamera::CreateVec(void)
{
	if (obj_ == nullptr) { return; }

	// 前ベクトルは追従オブジェクトと自分の距離
	front_ = obj_->GetTransform().position - pos_;
	// 前ベクトルの正規化
	front_ = VecNorm(front_);
	frontXPlane_ = VecNorm(VECTOR3(front_.x, 0, front_.z));
	at_ = obj_->GetTransform().position + (VecNorm(frontXPlane_) * 10) + (up_ * 15);
}

// 入力処理
void AtObjCamera::Input(void)
{
	if (parent_ && parent_->GetMainNum() != number_) { return; }
	const auto& ctrl = systems_->GetInput()->GetCtrl(0);

	// 入力を検知
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

	// 横回転
	rot_.x += ((ROTATION_SPEED * 2) * rAxis.x * fabs((float)rAxis.x));
	// 縦回転
	rot_.y += -(ROTATION_SPEED      * rAxis.y * fabs((float)rAxis.y));

	// 慣性
	rot_ *= 0.7f;

	rot_.x = max(min(rot_.x, MAX_ROTATION_SPEED), -MAX_ROTATION_SPEED);
	rot_.y = max(min(rot_.y, MAX_ROTATION_SPEED), -MAX_ROTATION_SPEED);
}

// 行列処理
void AtObjCamera::CreateMtx(void)
{
	if (obj_ == nullptr) { return; }

	// プレイヤー行列の生成
	VECTOR3 moveBase = obj_->GetTransform().position;
	moveBase.y += 10;
	MATRIX mtxObj;
	mtxObj.Identity().Translation(moveBase);

	// 移動
	MATRIX mtx;
	mtx.Identity().Translation(localPosition_);
	// 右ベクトルの生成
	right_ = VecCross(front_, up_);
	// 右ベクトルの正規化
	right_ = VecNorm(right_);

	rightXPlane_ = VecNorm(VECTOR3(right_.x, 0, right_.z));

	// プレイヤーの位置から後ろに下がる
	mtx *= mtxObj;

	pos_ = VECTOR3(mtx._41, mtx._42, mtx._43);
}

// 移動処理
void AtObjCamera::Move(void)
{
	// 速度が 0でない時
	if (fabs(rot_.x) > 0.0001f || fabs(rot_.y) > 0.0001f)
	{
		MATRIX mtxRotX, mtxRotY = MATRIX().Identity();
		// 横回転
		mtxRotX.Identity().Rotation(VECTOR3(0, rot_.x, 0));
		// 縦回転(制限を付けないと真上真下に来る(速度を加えないと戻れなくなる))
		if (((front_.y < 0.9f) && rot_.y < 0) || ((front_.y > -0.9f) && rot_.y > 0))
		{
			mtxRotY.Rotation(right_, rot_.y);
		}
		else { rot_.y = 0; }
		
		// ベクトルの座標変換
		localPosition_ = VecTransformNormal(localPosition_, mtxRotX);
		localPosition_ = VecTransformNormal(localPosition_, mtxRotY);
	}
	else
	{
		rot_ = VECTOR2(0, 0);
	}
}
