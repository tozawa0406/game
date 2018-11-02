//-----------------------------------------------------------------------------
//
//	�J�����Ǘ�[CameraManager.cpp]
//	Auther : ���V�đ�
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

// �R���X�g���N�^
CameraManager::CameraManager(SceneManager* parent) : parent_(parent), mainCamera_(0), cameraNum_(0)
{
	// ������
	for (int i = 0; i < MAX_CAMERA; i++)
	{
		camera_[i] = nullptr;
	}

	camera_[cameraNum_] = new Camera(this, cameraNum_);
	cameraNum_++;

//	debugCamera_ = new MoveCamera(this, -1);
}

// �f�X�g���N�^
CameraManager::~CameraManager(void)
{
	// �J�����̍폜
	for (auto obj : camera_) { DeletePtr(obj); }
//	DeletePtr(debugCamera_);
}

void CameraManager::Draw(void)
{
	// View�s���Proj�s��̐���
	CreateMatrix();
}

// �X�V����
int CameraManager::Update(void)
{
	// �f�o�b�O�J�����ւ̈ڍs
	DebugMove();

	// �J�����̍X�V����
	for (auto obj : camera_)
	{
		if (obj != nullptr)
		{
			obj->Update();
		}
	}
//	debugCamera_->Update();

	// ���C���J�����̕ύX
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

// �f�o�b�O�J�����ւ̈ڍs
void CameraManager::DebugMove(void)
{
//	VECTOR3 temp = { 0, 0, 0 };
//	// ImGui�őI������Ă���I�u�W�F�N�g��pos
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
//	// �I�΂�Ă�����
//	if (*selectObj != nullptr)
//	{
//		// �����_�̕ύX
//		if (!look)
//		{
//			// �t���O
//			look = true;
//			// ���݂̃J������pos��at���f�o�b�O�p�̃J�����ɐݒ�
//			debugCamera->SetCamera(camera[mainCamera]);
//			// ���݃J������pos��at��ۑ�
//			posOld = camera[mainCamera]->GetPos();
//			atOld  = camera[mainCamera]->GetAt();
//			// ���C���J�����̐ݒ��z��O��
//			mainOld = mainCamera;
//			mainCamera = -1;
//			// �I�����ꂽ�I�u�W�F�N�g�ƌ��݂̒����_�̋������ړ��Ɋ|����t���[�����Ŋ���
//			diff = (*selectObj - debugCamera->GetAt()) / (float)GuiManager::SELECT_GUI;
//		}
//
//		// �K��̃t���[����
//		if (frame++ < GuiManager::SELECT_GUI)
//		{
//			temp = debugCamera->GetAt() + diff;
//			// �����_�� 1�t���[���̈ړ����������ړ�
//			debugCamera->SetAt(temp);
//		}
//	}
//	// �I�΂�Ă��Ȃ����Ƀt���O���オ���Ă�����(�I��������)
//	else if (look)
//	{
//		if (frame > 0)
//		{
//			// �t���[���������}�C�i�X�ɂ���
//			frame = -GuiManager::SELECT_GUI;
//			diff = (atOld - debugCamera->GetAt()) / (float)GuiManager::SELECT_GUI;
//			diffPos = (posOld - debugCamera->GetPos()) / (float)GuiManager::SELECT_GUI;
//		}
//		// �K��̃t���[����
//		else if (frame++ < 0)
//		{
//			// �����_�̈ړ�
//			temp = debugCamera->GetAt() + diff;
//			debugCamera->SetAt(temp);
//			// �ʒu�̈ړ�
//			temp = debugCamera->GetPos() + diffPos;
//			debugCamera->SetPos(temp);
//		}
//		// �K��̃t���[�������I������
//		else
//		{
//			// ���C���J������߂�
//			mainCamera = mainOld;
//			// �t���O��߂�
//			look = false;
//		}
//	}
}

// �s��̍쐬
void CameraManager::CreateMatrix(void)
{
	// ���C���J�����̎擾
	Camera* main = nullptr;
	if (mainCamera_ >= 0)      { main = camera_[mainCamera_]; }
//	else if(mainCamera_ == -1) { main = debugCamera_;        }

//	int nowNumber = parent->GetDirectX()->GetNowNumber();
//	main = camera[nowNumber];

	if (main == nullptr) { return; }

	// �r���[�ϊ�
	const auto& dev = parent_->GetSystems()->GetRenderer()->GetWrapper();
	mtxView_ = dev->CreateViewMatrix(main->GetPos(), main->GetAt(), main->GetUp());
	//�v���W�F�N�V�����s��̍쐬
	mtxProj_ = dev->CreateProjectionMatrix(Camera::FOV, (float)Graphics::WIDTH / Graphics::HEIGHT, Camera::C_NEAR, Camera::C_FAR);
}

// �Ǐ]�J�����̍쐬
Camera* CameraManager::CreateAtObjCamera(Object* obj, int i)
{
	// �J�����̌����ő吔�Ȃ�
	if (i >= MAX_CAMERA) { return nullptr; }

	// �Ǐ]�J�����̍쐬
	AtObjCamera* temp = new AtObjCamera(this, cameraNum_, VECTOR3(0, 15, -10), obj);
	// �쐬�������̂�z��ɓ����
	if (camera_[i] != nullptr)
	{
		DeletePtr(camera_[i]);
	}
	camera_[i] = temp;
	cameraNum_++;

	return temp;
}

// �Ǐ]�J�����̍폜
void CameraManager::DestroyObjCamera(Camera* obj)
{
	// �z��̒�����J������T��
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

// Get����
Camera* CameraManager::GetCamera(void)
{
//	int temp = parent->GetDirectX()->GetNowNumber();
//	return camera[temp];

	if (mainCamera_ >= 0) { return camera_[mainCamera_]; }
//	else { return debugCamera_; }

	return nullptr;
}
