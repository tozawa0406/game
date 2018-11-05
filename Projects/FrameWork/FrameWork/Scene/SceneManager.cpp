//-----------------------------------------------------------------------------
//
//	�V�[������[SceneManager.cpp]
//	Auther : ���V�đ�
//																	2017/09/05
//-----------------------------------------------------------------------------
#include "SceneManager.h"
#include "../Windows/Windows.h"
#include "../Systems/GameSystems.h"

#include "EachScene/TitleScene.h"
#include "../../../Sources/Scene/EachScene/GameScene.h"
#include "EachScene/ResultScene.h"
#include "EachScene/PauseScene.h"

#include "../Systems/Input/KeyInput.h"
#include "../Systems/Input/Controller.h"
#include "../Systems/Camera/CameraManager.h"

// �R���X�g���N�^
SceneManager::SceneManager(Systems* systems) : Interface(systems), sceneChange_(CHANGE_SCENE), displayMode_(Scene::Num::MAX)
											 , scene_(nullptr), pause_(nullptr), fadeCnt_(SCENE_FADE_OUT - 1), isPause_(false), startLoad_(false)
{
	camera_  = new CameraManager(this);

#ifdef _DEBUG
	displayMode_ = Scene::Num::TITLE;
#endif
	fade_.Init(systems_, 250, (int)Texture::Base::FILL_RECTANGLE);
	fade_.position = { (float)Half(Graphics::WIDTH), (float)Half(Graphics::HEIGHT) };
	fade_.size     = { (float)Graphics::WIDTH, (float)Graphics::HEIGHT };
	fade_.color    = COLOR(0, 0, 0, 0);
	fade_.enable = false;

	loading_ = new Loading(systems_);
	loading_->Init();
}

// �f�X�g���N�^
SceneManager::~SceneManager(void)
{
	loading_->Uninit();
	DeletePtr(loading_);
	DeletePtr(pause_);
	DeletePtr(scene_);
	DeletePtr(camera_);
}

// �X�V����
void SceneManager::Update(void)
{	
	// �V�[���̍X�V
	SceneUpdate();

	// ���ۂ̃V�[���J�ڏ���
	ChangeActual();

	// �����V�[���J��
	ForceSceneChange();
}

// �`�揈��
void SceneManager::Draw(void)
{
	camera_->Draw();

	// �t�F�[�h����
	Fade();
}

// �V�[���̍X�V����
void SceneManager::SceneUpdate(void)
{	
	if (!isPause_)
	{
		camera_->Update();
		if (!GetSystems()->GetDebug()->GetDebugPause())
		{
			if (scene_)
			{
				Change(scene_->Update());
			}
		}
	}
	else
	{
		if (pause_)
		{
			switch (pause_->Update())
			{
			case 1:
				isPause_ = false;
				break;
			case 2:
				Change(CHANGE_GAME);
				DeletePtr(pause_);
				break;
			case 3:
				Change(CHANGE_TITLE);
				DeletePtr(pause_);
				break;
			}
		}
	}
}

// �t�F�[�h����
void SceneManager::Fade(void)
{
	int a = 0;
	if (fade_.enable)
	{
		fade_.enable = false;
		loading_->EndFade();
	}
	if (fadeCnt_ < SCENE_FADE_OUT)
	{
		//�b������ 1�t���[�����̃���ݒ�
		a = (255 / SCENE_FADE_OUT) * fadeCnt_;
	}
	else
	{
		//�b������ 1�t���[�����̃���ݒ�
		a = 255 - (255 / SCENE_FADE_IN) * (fadeCnt_ - SCENE_FADE_OUT);
	}
	//�V�[���؂�ւ����̈Ó]
	if (sceneChange_ & CHANGE_CHECK)
	{
		COLOR c = { 0, 0, 0, a / 255.0f };
		fade_.color = c;
		fade_.enable = true;
		loading_->FadeAlpha(c.a);
	}
}

// �O���ďo���J�ڏ�������
void SceneManager::Change(int scene)
{
	//�V�[���J�ڃt���O��true
	if (!sceneChange_)
	{
		sceneChange_ = scene;
	}
}

// ���ۂɃV�[�����؂�ւ�鏈��
void SceneManager::ChangeActual(void)
{
	//�O���ďo�����ꂽ��
	if (sceneChange_)
	{
		//�t�F�[�h�̐i�s
		fadeCnt_++;
		//�t�F�[�h�̏I�����Ԃ��z������
		if (fadeCnt_ == SCENE_FADE_OUT)
		{
			if (!startLoad_)
			{
				isPause_ = false;			//�|�[�Y��Ԃ�����

				if (sceneChange_ & CHANGE_SCENE)
				{
					//�V�[���J��
					displayMode_ = (Scene::Num)((int)displayMode_ + 1);

					if (displayMode_ > Scene::Num::RESULT)
					{//���U���g���z������^�C�g���ɖ߂�
						displayMode_ = Scene::Num::TITLE;
					}
				}
				else if (sceneChange_ & CHANGE_TITLE)
				{
					displayMode_ = Scene::Num::TITLE;
				}
				else if (sceneChange_ & CHANGE_GAME)
				{
					displayMode_ = Scene::Num::GAME;
				}

				DeletePtr(scene_);
				DeletePtr(pause_);
				loading_->Start((int)displayMode_);
				startLoad_ = true;
			}
			if (!loading_->IsLoading())
			{
				loading_->End();
				startLoad_ = false;
				scene_ = this->CreateScene();
			}
			if (startLoad_)
			{
				fadeCnt_--;
			}
		}
		else if (fadeCnt_ > SCENE_FADE_OUT + SCENE_FADE_IN)
		{
			//�t�F�[�h�I��
			sceneChange_ = CHANGE_NORMAL;
			fadeCnt_ = 0;
		}
	}
}

// BACK�{�^���őJ��
void SceneManager::ForceSceneChange(void)
{
#ifdef _SELF_DEBUG
	if(systems_->GetInput()->GetCtrl(0)->Press(Input::GAMEPAD_HOME, DIK_BACK))
	{
		Change(); 
	}
#endif
}

BaseScene* SceneManager::CreateScene(void)
{
	switch (displayMode_)
	{
	case Scene::Num::TITLE:
		return new TitleScene(this);
	case Scene::Num::GAME:
		pause_ = new Pause(this);
		return new GameScene(this);
	case Scene::Num::RESULT:
		return new ResultScene(this);
	}
	return nullptr;
}
