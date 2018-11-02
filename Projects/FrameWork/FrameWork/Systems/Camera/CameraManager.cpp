//-----------------------------------------------------------------------------
//
//	カメラ管理[CameraManager.cpp]
//	Auther : 戸澤翔太
//																	2018/01/24
//-----------------------------------------------------------------------------
#include "CameraManager.h"
#include "../../Windows/Windows.h"
#include "../GameSystems.h"
#include "../Input/KeyInput.h"
#include "../../Scene/SceneManager.h"
#include "../DebugSetting/GuiManager.h"
#include "MoveCamera.h"
#include "AtObjCamera.h"

// コンストラクタ
CameraManager::CameraManager(SceneManager* parent) : parent_(parent), mainCamera_(0), cameraNum_(0)
{
	// 初期化
	for (int i = 0; i < MAX_CAMERA; i++)
	{
		camera_[i] = nullptr;
	}

	camera_[cameraNum_] = new Camera(this, cameraNum_);
	cameraNum_++;

//	debugCamera_ = new MoveCamera(this, -1);
}

// デストラクタ
CameraManager::~CameraManager(void)
{
	// カメラの削除
	for (auto obj : camera_) { DeletePtr(obj); }
//	DeletePtr(debugCamera_);
}

void CameraManager::Draw(void)
{
	// View行列をProj行列の生成
	CreateMatrix();
}

// 更新処理
int CameraManager::Update(void)
{
	// デバッグカメラへの移行
	DebugMove();

	// カメラの更新処理
	for (auto obj : camera_)
	{
		if (obj != nullptr)
		{
			obj->Update();
		}
	}
//	debugCamera_->Update();

	// メインカメラの変更
	//if (parent_->GetSystems()->GetInput()->GetKeyboard()->Trig(DIK_F) && mainCamera_ != -1)
	//{
	//	mainCamera_++;
	//	if (mainCamera_ >= cameraNum_)
	//	{
	//		mainCamera_ = 0;
	//	}
	//}
	return 0;
}

// デバッグカメラへの移行
void CameraManager::DebugMove(void)
{
//	VECTOR3 temp = { 0, 0, 0 };
//	// ImGuiで選択されているオブジェクトのpos
//	const VECTOR3* selectObj = nullptr;
//
//#ifdef _DEBUG
//	selectObj = parent_->GetSystems()->GetDebug()->GetGuiManager()->GetSelect();
//#endif
//	if (!parent->GetSystems()->GetDebug()->GetGuiManager()->GetLookObject())
//	{
//		selectObj = nullptr;
//	}
//
//	// 選ばれていたら
//	if (*selectObj != nullptr)
//	{
//		// 注視点の変更
//		if (!look)
//		{
//			// フラグ
//			look = true;
//			// 現在のカメラのposとatをデバッグ用のカメラに設定
//			debugCamera->SetCamera(camera[mainCamera]);
//			// 現在カメラのposとatを保存
//			posOld = camera[mainCamera]->GetPos();
//			atOld  = camera[mainCamera]->GetAt();
//			// メインカメラの設定を配列外へ
//			mainOld = mainCamera;
//			mainCamera = -1;
//			// 選択されたオブジェクトと現在の注視点の距離を移動に掛けるフレーム数で割る
//			diff = (*selectObj - debugCamera->GetAt()) / (float)GuiManager::SELECT_GUI;
//		}
//
//		// 規定のフレーム間
//		if (frame++ < GuiManager::SELECT_GUI)
//		{
//			temp = debugCamera->GetAt() + diff;
//			// 注視点を 1フレームの移動距離だけ移動
//			debugCamera->SetAt(temp);
//		}
//	}
//	// 選ばれていない時にフラグが上がっていたら(選択解除時)
//	else if (look)
//	{
//		if (frame > 0)
//		{
//			// フレーム数ををマイナスにする
//			frame = -GuiManager::SELECT_GUI;
//			diff = (atOld - debugCamera->GetAt()) / (float)GuiManager::SELECT_GUI;
//			diffPos = (posOld - debugCamera->GetPos()) / (float)GuiManager::SELECT_GUI;
//		}
//		// 規定のフレーム間
//		else if (frame++ < 0)
//		{
//			// 注視点の移動
//			temp = debugCamera->GetAt() + diff;
//			debugCamera->SetAt(temp);
//			// 位置の移動
//			temp = debugCamera->GetPos() + diffPos;
//			debugCamera->SetPos(temp);
//		}
//		// 規定のフレーム数を終えたら
//		else
//		{
//			// メインカメラを戻す
//			mainCamera = mainOld;
//			// フラグを戻す
//			look = false;
//		}
//	}
}

// 行列の作成
void CameraManager::CreateMatrix(void)
{
	// メインカメラの取得
	Camera* main = nullptr;
	if (mainCamera_ >= 0)      { main = camera_[mainCamera_]; }
//	else if(mainCamera_ == -1) { main = debugCamera_;        }

//	int nowNumber = parent->GetDirectX()->GetNowNumber();
//	main = camera[nowNumber];

	if (main == nullptr) { return; }

	// ビュー変換
	const auto& dev = parent_->GetSystems()->GetRenderer()->GetWrapper();
	mtxView_ = dev->CreateViewMatrix(main->GetPos(), main->GetAt(), main->GetUp());
	//プロジェクション行列の作成
	mtxProj_ = dev->CreateProjectionMatrix(Camera::FOV, (float)Graphics::WIDTH / Graphics::HEIGHT, Camera::C_NEAR, Camera::C_FAR);
}

// 追従カメラの作成
Camera* CameraManager::CreateAtObjCamera(Object* obj, int i)
{
	// カメラの個数が最大数なら
	if (i >= MAX_CAMERA) { return nullptr; }

	// 追従カメラの作成
	AtObjCamera* temp = new AtObjCamera(this, cameraNum_, VECTOR3(0, 15, -10), obj);
	// 作成したものを配列に入れる
	if (camera_[i] != nullptr)
	{
		DeletePtr(camera_[i]);
	}
	camera_[i] = temp;
	cameraNum_++;

	return temp;
}

// 追従カメラの削除
void CameraManager::DestroyObjCamera(Camera* obj)
{
	// 配列の中からカメラを探す
	for (int i = 0; i < MAX_CAMERA; ++i)
	{
		if (camera_[i] == obj)
		{
			VECTOR3 pos = obj->GetPos();
			VECTOR3 at  = obj->GetAt();
			DeletePtr(camera_[i]);
			camera_[i] = new Camera(this, i);
			break;
		}
	}
	cameraNum_--;
}

// Get処理
Camera* CameraManager::GetCamera(void)
{
//	int temp = parent->GetDirectX()->GetNowNumber();
//	return camera[temp];

	if (mainCamera_ >= 0) { return camera_[mainCamera_]; }
//	else { return debugCamera_; }

	return nullptr;
}
