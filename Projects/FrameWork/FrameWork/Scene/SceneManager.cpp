#include "SceneManager.h"
#include "../Windows/Windows.h"
#include "../Systems/GameSystems.h"

#include "../Systems/Input/KeyInput.h"
#include "../Systems/Input/Controller.h"
#include "../Systems/Camera/CameraManager.h"

SceneManager::SceneManager(Systems* systems) : Interface(systems)
	, sceneChange_(SceneList::NEXT)
	, displayMode_(SceneList::MAX)
	, scene_(nullptr)
	, pause_(nullptr)
	, eachScene_(nullptr)
	, fadeCnt_(SCENE_FADE_OUT - 1)
	, isPause_(false)
	, startLoad_(false)
{
}

SceneManager::~SceneManager(void)
{
}

HRESULT SceneManager::Init(void)
{
	camera_ = new CameraManager(this);
	assert(camera_);

#ifdef _DEBUG
	displayMode_ = SceneList::TITLE;
#endif
	fade_.Init(250, (int)Texture::Base::WHITE);
	fade_.SetPosition(VECTOR2((float)Half(Graphics::WIDTH), (float)Half(Graphics::HEIGHT)));
	fade_.SetSize(VECTOR2((float)Graphics::WIDTH, (float)Graphics::HEIGHT));
	fade_.SetColor(COLOR(0, 0, 0, 0));
	fade_.SetEnable(false);

	loading_ = new Loading(systems_);
	assert(loading_);
	if (loading_)
	{
		loading_->Init();
	}

	eachScene_ = new EachScene;
	assert(eachScene_);

	return S_OK;
}

void SceneManager::Uninit(void)
{
	fade_.Uninit();
	DeletePtr(eachScene_);
	UninitDeletePtr(loading_);
	UninitDeletePtr(pause_);
	UninitDeletePtr(scene_);
	DeletePtr(camera_);
}

void SceneManager::Update(void)
{	
	// �V�[���̍X�V
	SceneUpdate();

	// ���ۂ̃V�[���J�ڏ���
	ChangeActual();

	// �����V�[���J��
	ForceSceneChange();
}

void SceneManager::Draw(void)
{
	camera_->Draw();

	// �t�F�[�h����
	Fade();
}

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
			int returnPause = static_cast<int>(pause_->Update());
			if (int(returnPause) && eachScene_)
			{
				int change = eachScene_->ChangePause(returnPause);
				if (change == 0) { isPause_ = false; }
				else
				{
					Change(static_cast<SceneList>(change));
					UninitDeletePtr(pause_);
				}
			}
		}
	}
}

void SceneManager::Fade(void)
{
	int a = 0;
	if (fade_.GetEnable())
	{
		fade_.SetEnable(false);
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
	if (static_cast<int>(sceneChange_))
	{
		COLOR c = { 0, 0, 0, a / 255.0f };
		fade_.SetColor(c);
		fade_.SetEnable(true);
		loading_->FadeAlpha(c.a);
	}
}

void SceneManager::Change(SceneList scene)
{
	//�V�[���J�ڃt���O��true
	if (sceneChange_ == SceneList::NOTCHANGE)
	{
		sceneChange_ = scene;
	}
}

void SceneManager::ChangeActual(void)
{
	//�O���ďo�����ꂽ��
	if (static_cast<int>(sceneChange_))
	{
		//�t�F�[�h�̐i�s
		fadeCnt_++;
		//�t�F�[�h�̏I�����Ԃ��z������
		if (fadeCnt_ == SCENE_FADE_OUT)
		{
			if (!startLoad_)
			{
				isPause_ = false;			//�|�[�Y��Ԃ�����

				if (eachScene_)
				{
					eachScene_->ChangeScene(sceneChange_, displayMode_);
				}

				UninitDeletePtr(scene_);
				UninitDeletePtr(pause_);
				loading_->Start((int)displayMode_);
				startLoad_ = true;
			}
			if (!loading_->IsLoading())
			{
				loading_->End();
				startLoad_ = false;
				if (eachScene_)
				{
					scene_ = eachScene_->CreateScene(this, displayMode_);
					pause_ = eachScene_->CreatePause(this, displayMode_);
				}
			}
			if (startLoad_)
			{
				fadeCnt_--;
			}
		}
		else if (fadeCnt_ > SCENE_FADE_OUT + SCENE_FADE_IN)
		{
			//�t�F�[�h�I��
			sceneChange_ = SceneList::NOTCHANGE;
			fadeCnt_ = 0;
		}
	}
}

void SceneManager::ForceSceneChange(void)
{
#ifdef _SELF_DEBUG
	if(systems_->GetInput()->GetCtrl(0)->Press(Input::GAMEPAD_HOME, DIK_F5))
	{
		Change(); 
	}
#endif
}
