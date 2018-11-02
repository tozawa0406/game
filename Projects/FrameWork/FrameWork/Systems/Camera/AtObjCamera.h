//-----------------------------------------------------------------------------
//
//	オブジェクト追従カメラ[AtObjCamera.h]
//	Auther : 戸澤翔太
//																	2018/01/24
//-----------------------------------------------------------------------------
#ifndef _AT_OBJ_CAMERA_H_
#define _AT_OBJ_CAMERA_H_

#include "../../Define/Define.h"
#include "Camera.h"

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class MobileSuits;
class AtObjCamera : public Camera
{
	friend class CameraManager;

	static constexpr float ROTATION_SPEED				= -0.00175f;
	static constexpr float MAX_ROTATION_SPEED			= 0.05f;
	static constexpr float ROTATION_AXIS_COEFFICIENT	= 1.0f / 25.0f;

public:	
	// デストラクタ
	~AtObjCamera(void);

	// 更新処理
	void Update(void);
	// ImGui処理(デバッグ時のみ)
	void GuiUpdate(void);

	void DestroyObject(void) { obj_ = nullptr; }

private:
	// コンストラクタ
	AtObjCamera(CameraManager* parent, int number, VECTOR3 pos, Object* obj);
	// ベクトル生成処理
	void CreateVec(void);
	// 入力処理
	void Input(void);
	// 行列処理
	void CreateMtx(void);
	// 移動処理
	void Move(void);

	// 追従オブジェクト
	Object* obj_;

	VECTOR3 localPosition_;
	// 回転(標的がいないときに使用)
	VECTOR2 rot_;
};

#endif // _AT_OBJ_CAMERA_H_