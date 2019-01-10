#include "PauseScene.h"
#include <FrameWork/Scene/SceneManager.h>
#include <FrameWork/Windows/Windows.h>

/* @brief	�I�����̏��				*/
static constexpr int PAUSE_SELECT_UP		= 0;
/* @brief	�I�����Ă���̓_�Ŏ���		*/
static constexpr int PADDING_TIME			= 30;

Pause::Pause(void) : GUI(Systems::Instance(), nullptr, "ScenePause")
	 , selectNum_(0)
	 , paddingTimeCnt_(0)
{
	 for (auto& p : pos_) { p = VECTOR2(0, 0); }
}

Pause::~Pause(void)
{
}

void Pause::Init(void)
{
	 // �Ó]
	 back_.SetPosition(VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT)));
	 back_.SetSize(VECTOR2(Windows::WIDTH, Windows::HEIGHT));
	 back_.SetColor(COLOR::RGBA(0, 0, 0, 100));
	 back_.Init(250, (int)Texture::Base::WHITE);

	 string temp[3] = { "�߂�", "��蒼��", "�^�C�g����" };
	 float  adjust[3] = { 1, 2, 2.5f };
	 // ���j���[
	 for (int i = 0; i < 3; ++i)
	 {
		 // ���j���[�w�i
		 menuBack_[i].SetColor(COLOR(1, 1, 1, 1));
		 menuBack_[i].SetPosition(VECTOR2(Half(Windows::WIDTH), PAUSE_H * (1.0f + (2 * i)) + Half(PAUSE_H)));
		 menuBack_[i].SetSize(VECTOR2(PAUSE_W, PAUSE_H));
		 menuBack_[i].Init(252, (int)Texture::Base::WHITE);

		 // 
		 menu_[i].SetPosition(menuBack_[i].GetPosition() - VECTOR2(70 * adjust[i], 35));
		 menu_[i].SetColor(COLOR::RGBA(100, 100, 100, 255));
		 menu_[i].SetSize(menuBack_[i].GetSize());
		 menu_[i].Init(253, temp[i], 70);

		 if (i < 2)
		 {
			 // ��I���I�u�W�F�N�g���Â����邽��
			 notSelect_[i].Init(254, (int)Texture::Base::WHITE);
			 notSelect_[i].SetPosition(VECTOR2(Half(Windows::WIDTH), PAUSE_H * (1.0f + (2 * (i + 1))) + Half(PAUSE_H)));
			 notSelect_[i].SetSize(menuBack_[i].GetSize());
			 notSelect_[i].SetColor(COLOR::RGBA(0, 0, 0, 150));
		 }
	 }
	 EnableUI(false);
}

void Pause::Uninit(void)
{
	for (auto& b : menu_)		{ b.Uninit(); }
	for (auto& b : notSelect_)	{ b.Uninit(); }
	for (auto& b : menuBack_)	{ b.Uninit(); }
	back_.Uninit();
}

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
			menuBack_[selectNum_].SetColor(COLOR(1, 1, 1, 1));
		}
		else
		{
			menuBack_[selectNum_].SetColor(COLOR::RGBA(100, 100, 100, 200));
		}

		return SceneList::NOTCHANGE;
	}

	const auto& sound = GetSound();
	if (!sound) { return SceneList::NOTCHANGE; }

	//�㉺�L�[����
	if (GetCtrl(0)->Trigger(Input::AXIS_LY, DIK_S, true) < 0 || GetCtrl(0)->Trigger(Input::GAMEPAD_DOWN, DIK_DOWN))
	{
		if (selectNum_ < SELECT_NUM - 1)
		{
			sound->Play((int)Sound::Base::SE_SELECT);
			selectNum_++;
		}
	}
	else if (GetCtrl(0)->Trigger(Input::AXIS_LY, DIK_W) > 0 || GetCtrl(0)->Trigger(Input::GAMEPAD_UP, DIK_UP))
	{
		if (selectNum_ > PAUSE_SELECT_UP)
		{
			sound->Play((int)Sound::Base::SE_SELECT);
			selectNum_--;
		}
		
	}

	//����L�[
	if (GetCtrl(0)->Press(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		sound->Play((int)Sound::Base::SE_ENTER);
		paddingTimeCnt_++;
	}

	if (GetCtrl(0)->Trigger(Input::GAMEPAD_START, DIK_P))
	{
		sound->Play((int)Sound::Base::SE_SELECT);
		selectNum_ = 0;
		EnableUI(false);
		manager_->SetPause(false);
		return SceneList::NOTCHANGE;
	}

	int not = 0;
	// ��I���I�u�W�F�N�g�̑I��
	for (int i = 0; i < SELECT_NUM; ++i)
	{
		if (i != selectNum_)
		{
			notSelect_[not].SetPosition(menuBack_[i].GetPosition());
			not++;
		}
	}

	return SceneList::NOTCHANGE;
}

void Pause::EnableUI(bool enable)
{
	back_.SetEnable(enable);
	for (int i = 0; i < SELECT_NUM; ++i)
	{
		menu_[i].SetEnable(enable);
		menuBack_[i].SetEnable(enable);
		if (i < SELECT_NUM - 1)
		{
			notSelect_[i].SetEnable(enable);
		}
	}
}
