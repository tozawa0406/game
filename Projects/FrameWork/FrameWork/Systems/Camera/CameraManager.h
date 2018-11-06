//-----------------------------------------------------------------------------
//
//	カメラ管理[CameraManager.h]
//	Auther : 戸澤翔太
//																	2018/01/24
//-----------------------------------------------------------------------------
#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_

#include "../../Define/Define.h"
#include "../../Scene/SceneManager.h"
#include "Camera.h"
#include "../GameSystems.h"

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class MoveCamera;
class AtObjCamera;
class MobileSuits;
class CameraManager
{
	friend class SceneManager;
public:
	// カメラの個数
	static constexpr int MAX_CAMERA = 2;

	// デストラクタ
	~CameraManager(void);

	int Update(void);		// 更新処理
	void Draw(void);

	/* @fn		ChangeCamera
	 * @brief	メインカメラの変更
	 * @param	変更したいカメラの配列番号			*/
	inline void ChangeCamera(int cameraNum) { (cameraNum < MAX_CAMERA) ? mainCamera_ = cameraNum : cameraNum; }

	// Getter
		   Camera*       GetCamera(void);
	inline MATRIX&       GetView(void)         { return mtxView_;    }
	inline MATRIX&       GetProj(void)         { return mtxProj_;    }
	inline SceneManager* GetSceneManager(void) { return parent_;     }
	inline int           GetMainNum(void)      { return mainCamera_; }
	inline bool          GetLook(void)         { return look_;       }

	// デバッグカメラへの移行
	void DebugMove(void);

	// 追従カメラの作成
	Camera* CreateAtObjCamera(Object* obj, int i, VECTOR3 pos = VECTOR3(0, 15, -10));
	// 追従カメラの削除
	void	     DestroyObjCamera(Camera* obj);

private:
	// コンストラクタ
	CameraManager(SceneManager* parent);

	// 行列変換
	void CreateMatrix(void);

	// 子
	Camera* camera_[MAX_CAMERA];
	Camera* debugCamera_;

	// 行列
	MATRIX  mtxView_;
	MATRIX  mtxProj_;

	// メインカメラ
	int  mainCamera_;
	int  cameraNum_;

	// デバッグ時に使用
	bool look_;
	int  frame_;
	int  mainOld_;
	VECTOR3 atOld_;
	VECTOR3 posOld_;
	VECTOR3 diff_;
	VECTOR3 diffPos_;

	SceneManager* parent_;
};

#endif // _CAMERA_H_