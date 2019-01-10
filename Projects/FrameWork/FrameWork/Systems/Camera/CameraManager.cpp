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
CameraManager::CameraManager(SceneManager* parent) : parent_(parent), mainCamera_(0), cameraNum_(0), look_(false), frame_(0)
{
	// ������
	for (int i = 0; i < MAX_CAMERA; ++i)
	{
		camera_[i] = nullptr;
	}

	camera_[cameraNum_] = new Camera(this, cameraNum_);
	cameraNum_++;

	debugCamera_ = new MoveCamera(this, -1);
}

// �f�X�g���N�^
CameraManager::~CameraManager(void)
{
	// �J�����̍폜
	for (auto obj : camera_) { DeletePtr(obj); }
	DeletePtr(debugCamera_);
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
	debugCamera_->Update();
	return 0;
}

// �f�o�b�O�J�����ւ̈ڍs
void CameraManager::DebugMove(void)
{
	VECTOR3 temp = { 0, 0, 0 };
	// ImGui�őI������Ă���I�u�W�F�N�g��pos
	const VECTOR3* selectObj = nullptr;

#ifdef _SELF_DEBUG
	selectObj = parent_->GetSystems()->GetDebug()->GetGuiManager()->GetSelect();
#endif
	if (!parent_->GetSystems()->GetDebug()->GetGuiManager()->GetLookObject())
	{
		selectObj = nullptr;
	}

	// �I�΂�Ă�����
	if (*selectObj != nullptr)
	{
		// �����_�̕ύX
		if (!look_)
		{
			// �t���O
			look_ = true;
			// ���݂̃J������pos��at���f�o�b�O�p�̃J�����ɐݒ�
			debugCamera_->SetCamera(camera_[mainCamera_]);
			// ���݃J������pos��at��ۑ�
			posOld_ = camera_[mainCamera_]->GetPos();
			atOld_  = camera_[mainCamera_]->GetAt();
			// ���C���J�����̐ݒ��z��O��
			mainOld_    = mainCamera_;
			mainCamera_ = -1;
			// �I�����ꂽ�I�u�W�F�N�g�ƌ��݂̒����_�̋������ړ��Ɋ|����t���[�����Ŋ���
			diff_ = (*selectObj - debugCamera_->GetAt()) / (float)GuiManager::SELECT_GUI;
		}

		// �K��̃t���[����
		if (frame_++ < GuiManager::SELECT_GUI)
		{
			temp = debugCamera_->GetAt() + diff_;
			// �����_�� 1�t���[���̈ړ����������ړ�
			debugCamera_->SetAt(temp);
		}
	}
	// �I�΂�Ă��Ȃ����Ƀt���O���オ���Ă�����(�I��������)
	else if (look_)
	{
		if (frame_ > 0)
		{
			// �t���[���������}�C�i�X�ɂ���
			frame_ = -GuiManager::SELECT_GUI;
			diff_ = (atOld_ - debugCamera_->GetAt()) / (float)GuiManager::SELECT_GUI;
			diffPos_ = (posOld_ - debugCamera_->GetPos()) / (float)GuiManager::SELECT_GUI;
		}
		// �K��̃t���[����
		else if (frame_++ < 0)
		{
			// �����_�̈ړ�
			temp = debugCamera_->GetAt() + diff_;
			debugCamera_->SetAt(temp);
			// �ʒu�̈ړ�
			temp = debugCamera_->GetPos() + diffPos_;
			debugCamera_->SetPos(temp);
		}
		// �K��̃t���[�������I������
		else
		{
			// ���C���J������߂�
			mainCamera_ = mainOld_;
			// �t���O��߂�
			look_ = false;
		}
	}
}

// �s��̍쐬
void CameraManager::CreateMatrix(void)
{
	// ���C���J�����̎擾
	Camera* main = nullptr;
	if (mainCamera_ >= 0)      { main = camera_[mainCamera_]; }
	else if(mainCamera_ == -1) { main = debugCamera_;        }

	if (main == nullptr) { return; }

	// �r���[�ϊ�
	mtxView_ = CreateViewMatrix(main->GetPos(), main->GetAt(), main->GetUp());
	//�v���W�F�N�V�����s��̍쐬
	mtxProj_ = CreateProjectionMatrix(Camera::FOV, Windows::WIDTH / (float)Windows::HEIGHT, Camera::C_NEAR, Camera::C_FAR);
}

// �Ǐ]�J�����̍쐬
Camera* CameraManager::CreateAtObjCamera(Object* obj, int i, VECTOR3 pos)
{
	// �J�����̌����ő吔�Ȃ�
	if (i >= MAX_CAMERA) { return nullptr; }

	// �Ǐ]�J�����̍쐬
	AtObjCamera* temp = new AtObjCamera(this, i, pos, obj);
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
	if (mainCamera_ >= 0) { return camera_[mainCamera_]; }
	else { return debugCamera_; }
}
