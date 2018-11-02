//-----------------------------------------------------------------------------
//
//	�V�X�e���n[GameSystems.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "GameSystems.h"
#include "../Windows/Windows.h"
#include "../Graphics/Graphics.h"
#include "../Scene/SceneManager.h"
#include "Screenshot.h"

Systems* Systems::system_;

Systems::Systems(Windows* window) : window_(window), graphics_(nullptr)
{
	graphics_ = window->GetGraphics();
}

HRESULT Systems::Init(void)
{
	system_ = this;

	for (auto& i : manager_)
	{
		i = nullptr;
	}

	// �C���X�^���X���� & ������
	if (!Create<Debug>					 ((int)SystemsNum::DEBUG          , "�f�o�b�O�@�\"		)) { return E_FAIL; }
	if (!Create<Input>					 ((int)SystemsNum::INPUT          , "����"              )) { return E_FAIL; }
	if (!Create<CanvasRendererManager>	 ((int)SystemsNum::CANVASRENDERER , "2D�`��"            )) { return E_FAIL; }
	if (!Create<ObjectRendererManager>	 ((int)SystemsNum::OBJECTRENDERER , "3D�`��"			)) { return E_FAIL; }
	if (!Create<Texture>				 ((int)SystemsNum::TEXTURE        , "�e�N�X�`��"		)) { return E_FAIL; }
	if (!Create<Model>					 ((int)SystemsNum::MODEL		  , "���f��"			)) { return E_FAIL; }
	if (!Create<ShaderManager>			 ((int)SystemsNum::SHADER         , "�V�F�[�_�["		)) { return E_FAIL; }
	if (!Create<Sound>					 ((int)SystemsNum::SOUND          , "�T�E���h"			)) { return E_FAIL; }
	if (!Create<Collision2DManager>		 ((int)SystemsNum::COLLISION2D    , "2D�����蔻��"		)) { return E_FAIL; }
	if (!Create<Collision3DManager>		 ((int)SystemsNum::COLLISION3D    , "3D�����蔻��"		)) { return E_FAIL; }
	if (!Create<ColliderRendererManager> ((int)SystemsNum::COLLISIONDRAW  , "�����蔻��`��"	)) { return E_FAIL; }
	if (!Create<ParticleManager>		 ((int)SystemsNum::PARTICLE		  , "�p�[�e�B�N��"		)) { return E_FAIL; }
	if (!Create<SceneManager>			 ((int)SystemsNum::SCENE          , "�V�[��"			)) { return E_FAIL; }
	if (!Create<Screenshot>				 ((int)SystemsNum::SCREENSHOT     , "�X�N���[���V���b�g")) { return E_FAIL; }

	return S_OK;
}

void Systems::Uninit(void)
{
	// �㏈��
	for (int i = (int)SystemsNum::MAX - 1; i >= 0; --i)
	{
		if (manager_[i])
		{
			manager_[i]->Uninit();
			DeletePtr(manager_[i]);
		}
	}
}

void Systems::Update(void)
{
	for (int i = 0; i < (int)SystemsNum::MAX; ++i)
	{
		if (manager_[i])
		{
			manager_[i]->Update();
		}
	}
}

void Systems::Draw(void)
{
	// �e�̕`��
	const auto& objRenderer = (ObjectRendererManager*)manager_[(int)SystemsNum::OBJECTRENDERER];
	objRenderer->DrawShadow();

	window_->GetGraphics()->ClearRenderer();

	// �Q�[���̕`��
	const auto& scene = (SceneManager*)manager_[(int)SystemsNum::SCENE];
	scene->Draw();

	// �V���ƃt�B�[���h���ɕ`��
	objRenderer->FastDraw();

	for (int i = 0; i < (int)SystemsNum::MAX; ++i)
	{
		if (i == (int)SystemsNum::SCENE || i == (int)SystemsNum::DEBUG) { continue; }
		if (!manager_[i]) { continue; }
		manager_[i]->Draw();
	}

	manager_[(int)SystemsNum::DEBUG]->Draw();
}

template<class T>
bool Systems::Create(int i, string error)
{
	manager_[i] = new T(this);
	string message = error + "�̏������Ɏ��s���܂���";
	if (window_->ErrorMessage(message.c_str(), "�G���[", manager_[i]->Init()))
	{
		return false;
	}
	return true;
}
