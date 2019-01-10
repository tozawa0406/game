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
CameraManager::CameraManager(SceneManager* parent) : parent_(parent), mainCamera_(0), cameraNum_(0), look_(false), frame_(0)
{
	// 初期化
	for (int i = 0; i < MAX_CAMERA; ++i)
	{
		camera_[i] = nullptr;
	}

	camera_[cameraNum_] = new Camera(this, cameraNum_);
	cameraNum_++;

	debugCamera_ = new MoveCamera(this, -1);
}

// デストラクタ
CameraManager::~CameraManager(void)
{
	// カメラの削除
	for (auto obj : camera_) { DeletePtr(obj); }
	DeletePtr(debugCamera_);
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
	debugCamera_->Update();
	return 0;
}

// デバッグカメラへの移行
void CameraManager::DebugMove(void)
{
	VECTOR3 temp = { 0, 0, 0 };
	// ImGuiで選択されているオブジェクトのpos
	const VECTOR3* selectObj = nullptr;

#ifdef _SELF_DEBUG
	selectObj = parent_->GetSystems()->GetDebug()->GetGuiManager()->GetSelect();
#endif
	if (!parent_->GetSystems()->GetDebug()->GetGuiManager()->GetLookObject())
	{
		selectObj = nullptr;
	}

	// 選ばれていたら
	if (*selectObj != nullptr)
	{
		// 注視点の変更
		if (!look_)
		{
			// フラグ
			look_ = true;
			// 現在のカメラのposとatをデバッグ用のカメラに設定
			debugCamera_->SetCamera(camera_[mainCamera_]);
			// 現在カメラのposとatを保存
			posOld_ = camera_[mainCamera_]->GetPos();
			atOld_  = camera_[mainCamera_]->GetAt();
			// メインカメラの設定を配列外へ
			mainOld_    = mainCamera_;
			mainCamera_ = -1;
			// 選択されたオブジェクトと現在の注視点の距離を移動に掛けるフレーム数で割る
			diff_ = (*selectObj - debugCamera_->GetAt()) / (float)GuiManager::SELECT_GUI;
		}

		// 規定のフレーム間
		if (frame_++ < GuiManager::SELECT_GUI)
		{
			temp = debugCamera_->GetAt() + diff_;
			// 注視点を 1フレームの移動距離だけ移動
			debugCamera_->SetAt(temp);
		}
	}
	// 選ばれていない時にフラグが上がっていたら(選択解除時)
	else if (look_)
	{
		if (frame_ > 0)
		{
			// フレーム数ををマイナスにする
			frame_ = -GuiManager::SELECT_GUI;
			diff_ = (atOld_ - debugCamera_->GetAt()) / (float)GuiManager::SELECT_GUI;
			diffPos_ = (posOld_ - debugCamera_->GetPos()) / (float)GuiManager::SELECT_GUI;
		}
		// 規定のフレーム間
		else if (frame_++ < 0)
		{
			// 注視点の移動
			temp = debugCamera_->GetAt() + diff_;
			debugCamera_->SetAt(temp);
			// 位置の移動
			temp = debugCamera_->GetPos() + diffPos_;
			debugCamera_->SetPos(temp);
		}
		// 規定のフレーム数を終えたら
		else
		{
			// メインカメラを戻す
			mainCamera_ = mainOld_;
			// フラグを戻す
			look_ = false;
		}
	}
}

// 行列の作成
void CameraManager::CreateMatrix(void)
{
	// メインカメラの取得
	Camera* main = nullptr;
	if (mainCamera_ >= 0)      { main = camera_[mainCamera_]; }
	else if(mainCamera_ == -1) { main = debugCamera_;        }

	if (main == nullptr) { return; }

	// ビュー変換
	mtxView_ = CreateViewMatrix(main->GetPos(), main->GetAt(), main->GetUp());
	//プロジェクション行列の作成
	mtxProj_ = CreateProjectionMatrix(Camera::FOV, Windows::WIDTH / (float)Windows::HEIGHT, Camera::C_NEAR, Camera::C_FAR);
}

// 追従カメラの作成
Camera* CameraManager::CreateAtObjCamera(Object* obj, int i, VECTOR3 pos)
{
	// カメラの個数が最大数なら
	if (i >= MAX_CAMERA) { return nullptr; }

	// 追従カメラの作成
	AtObjCamera* temp = new AtObjCamera(this, i, pos, obj);
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
	if (mainCamera_ >= 0) { return camera_[mainCamera_]; }
	else { return debugCamera_; }
}
