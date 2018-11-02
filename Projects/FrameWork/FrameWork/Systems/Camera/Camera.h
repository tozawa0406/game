//-----------------------------------------------------------------------------
//
//	カメラ[Camera.h]
//	Auther : 戸澤翔太
//																	2017/11/07
//-----------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../../Define/Define.h"
#include "../DebugSetting/GUI.h"

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class Object;
class Systems;
class Camera : public GUI
{
	friend class CameraManager;
public:
	// 画角
	static constexpr int FOV = 80;
	// ニアクリップ
	static constexpr float C_NEAR = 0.1f;
	// ファークリップ
	static constexpr float C_FAR = 1000.0f;

	// カメラの初期座標
	static constexpr float POS_X = 0.0f;
	static constexpr float POS_Y = 10.0f;
	static constexpr float POS_Z = -50.0f;

	// デストラクタ
	virtual ~Camera(void) {};

	virtual void Update(void) {};			// 更新処理

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
	// コンストラクタ
	Camera(CameraManager* parent, int number, VECTOR3 pos = VECTOR3(POS_X, POS_Y, POS_Z),
		VECTOR3 at = VECTOR3(0.0f, 0.0f, 0.0f));

	// 親クラス
	CameraManager* parent_;

	int     number_;
	VECTOR3 pos_;		// カメラの位置座標
	VECTOR3 at_;		// 注視点座標
	VECTOR3 up_;		// カメラの上を表す
	VECTOR3 front_;		// 前ベクトル
	VECTOR3 right_;		// 右ベクトル

	VECTOR3 frontXPlane_;	//! 前ベクトルから高さ成分(y)を消した値
	VECTOR3 rightXPlane_;	//! 前ベクトルから高さ成分(y)を消した値

	Systems* systems_;
};

#endif // _CAMERA_H_