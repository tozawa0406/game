//-----------------------------------------------------------------------------
//
//	�|�[�Y[PauseScene.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "PauseScene.h"
#include <FrameWork/Scene/SceneManager.h>

// �R���X�g���N�^
 Pause::Pause(SceneManager* manager) : BaseScene(manager),  GUI(manager->GetSystems(), nullptr, "ScenePause")
	 , selectNum_(0)
	 , paddingTimeCnt_(0)
{
}

 // �f�X�g���N�^
 Pause::~Pause(void)
 {
 }

 void Pause::Init(void)
 {
	 // �Ó]
	 back_.position = VECTOR2((float)Half(Graphics::WIDTH), (float)Half(Graphics::HEIGHT));
	 back_.size = VECTOR2((float)Graphics::WIDTH, (float)Graphics::HEIGHT);
	 back_.color = COLOR::RGBA(0, 0, 0, 100);
	 back_.Init(systems_, 250, (int)Texture::Base::FILL_RECTANGLE);

	 // ���j���[
	 for (int i = 0; i < 3; ++i)
	 {
		 // ���j���[�w�i
		 menuBack_[i].color = COLOR(1, 1, 1, 1);
		 menuBack_[i].position = VECTOR2((float)Half(Graphics::WIDTH), PAUSE_H * (1.0f + (2 * i)) + Half(PAUSE_H));
		 menuBack_[i].size = VECTOR2(PAUSE_W, PAUSE_H);
		 menuBack_[i].Init(systems_, 252, (int)Texture::Base::FILL_RECTANGLE);

		 // 
		 menu_[i].position = menuBack_[i].position;
		 menu_[i].color = COLOR::RGBA(100, 100, 100, 255);
		 menu_[i].size = menuBack_[i].size;
		 menu_[i].pattern = (float)i;
		 menu_[i].split.y = 3;
		 //		menu_[i].Init(systems_, 253, (int)Texture::Game::MENU);

		 if (i < 2)
		 {
			 // ��I���I�u�W�F�N�g���Â����邽��
			 notSelect_[i].Init(systems_, 254, (int)Texture::Base::FILL_RECTANGLE);
			 notSelect_[i].position = VECTOR2((float)Half(Graphics::WIDTH), PAUSE_H * (1.0f + (2 * (i + 1))) + Half(PAUSE_H));
			 notSelect_[i].size = menu_[i].size;
			 notSelect_[i].color = COLOR::RGBA(0, 0, 0, 150);
		 }
	 }
	 EnableUI(false);
 }

 void Pause::Uninit(void)
 {
 }

 // �X�V����
 SceneList Pause::Update(void)
{
	EnableUI(true);

	// ���j���[�����̎���
	if (paddingTimeCnt_ > PADDING_TIME)
	{
		// �V�[���J��
		paddingTimeCnt_ = 0;
		if (selectNum_ == 0) { EnableUI(false); }
		return static_cast<SceneList>(selectNum_ + 1);
	}
	// �I�����Ă���J�ڂ܂ł̎���
	else if (paddingTimeCnt_ > 0) 
	{
		paddingTimeCnt_++;

		// �_�ŏ���
		if (paddingTimeCnt_ % 10 < 5)
		{
			menuBack_[selectNum_].color = COLOR(1, 1, 1, 1);
		}
		else
		{
			menuBack_[selectNum_].color = COLOR::RGBA(100, 100, 100, 200);
		}

		return SceneList::NOTCHANGE;
	}

	//�㉺�L�[����
	if (GetCtrl(0)->Trigger(Input::AXIS_LY, DIK_S, true) < 0 || GetCtrl(0)->Trigger(Input::GAMEPAD_DOWN))
	{
		if (selectNum_ < PAUSE_SELECT_DOWN)
		{
//			systems_->GetSound()->Play((int)Sound::Base::SE_SELECT);
			selectNum_++;
		}
	}
	else if (GetCtrl(0)->Trigger(Input::AXIS_LY, DIK_W) > 0 || GetCtrl(0)->Trigger(Input::GAMEPAD_UP))
	{
		if (selectNum_ > PAUSE_SELECT_UP)
		{
//			systems_->GetSound()->Play((int)Sound::Base::SE_SELECT);
			selectNum_--;
		}
		
	}

	//����L�[
	if (GetCtrl(0)->Press(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
//		systems_->GetSound()->Play((int)Sound::Base::SE_ENTER);
		paddingTimeCnt_++;
	}

	if (GetCtrl(0)->Trigger(Input::GAMEPAD_START, DIK_P))
	{
//		systems_->GetSound()->Play((int)Sound::Base::SE_SELECT);
		selectNum_ = 0;
		EnableUI(false);
		manager_->SetPause(false);
		return SceneList::NOTCHANGE;
	}

	int not = 0;
	// ��I���I�u�W�F�N�g�̑I��
	for (int i = 0; i < 3; ++i)
	{
		if (i != selectNum_)
		{
			notSelect_[not].position = menu_[i].position;
			not++;
		}
	}

	return SceneList::NOTCHANGE;
}

// UI�̎g�p����C�ɕύX
void Pause::EnableUI(bool enable)
{
	back_.enable = enable;
	for (int i = 0; i < 3; ++i)
	{
		menu_[i].enable = enable; 
		menuBack_[i].enable = enable;
		if (i < 2)
		{
			notSelect_[i].enable = enable;
		}
	}
}
