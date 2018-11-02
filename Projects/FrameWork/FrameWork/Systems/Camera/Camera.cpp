//-----------------------------------------------------------------------------
//
//	カメラ[Camera.cpp]
//	Auther : 戸澤翔太
//																	2017/11/07
//-----------------------------------------------------------------------------
#include "Camera.h"
#include "../../Windows/Windows.h"
#include "../../Scene/SceneManager.h"
#include "CameraManager.h"

// コンストラクタ
Camera::Camera(CameraManager* parent, int number, VECTOR3 pos, VECTOR3 at) : GUI(parent->GetSceneManager()->GetSystems(), nullptr, "Camera")
							, systems_(parent->GetSceneManager()->GetSystems()), parent_(parent), number_(number), pos_(pos), at_(at)
							, up_(VECTOR3(0, 1, 0)), right_(VECTOR3(1, 0, 0))
{
}
