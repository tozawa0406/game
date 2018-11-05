//-----------------------------------------------------------------------------
//
//	カメラ[Camera.h]
//	Auther : 戸澤翔太
//																	2017/11/07
//-----------------------------------------------------------------------------
#ifndef _MOVE_CAMERA_H_
#define _MOVE_CAMERA_H_

#include "../../Define/Define.h"
#include "Camera.h"

class MoveCamera : public Camera
{
	friend class CameraManager;

	// 移動速度
	static constexpr float MOVE_SPEED = 0.06f;
	// 回転速度
	static constexpr float ROTATION_SPEED = 0.002f;

public:
	// デストラクタ
	~MoveCamera(void);

	void Update(void);			// 更新処理
	void GuiUpdate(void);		// GUI

private:
	// コンストラクタ
	MoveCamera(CameraManager* parent, int number, VECTOR3 pos = VECTOR3(Camera::POS_X, Camera::POS_Y, Camera::POS_Z),
		VECTOR3 at = VECTOR3(0.0f, 0.0f, 0.0f));
	void Input(void);			// キー入力
	void MoveTrans(void);		// 平行移動
	void MoveRotation(void);	// 自分を中心に回転
	void MoveAtRotation(void);	// 注視点を中心に回転

	// 方向
	VECTOR2 dir = { 0, 0 };
	// 速度
	VECTOR2 velocity = { 0, 0 };
	VECTOR2 rotVelocity = { 0, 0 };
	VECTOR2 rotAtVelocity = { 0, 0 };
};

#endif // _CAMERA_H_