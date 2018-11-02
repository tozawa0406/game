#include "Screenshot.h"
#include "../Windows/Windows.h"
#include "GameSystems.h"
#include "Input/Controller.h"

Screenshot::Screenshot(Systems* systems) : Interface(systems), screenUICnt_(0), enable_(false)
{
}

Screenshot::~Screenshot(void)
{
	systems_->GetRenderer()->GetWrapper()->ReleaseScreenshot();
}

HRESULT Screenshot::Init(void)
{
	// �X�N���[���V���b�g�pUI����
	sprite_[0].Init(systems_, 254, (int)Texture::Base::SSUI);
	float s = Graphics::HEIGHT * 0.1f;
	sprite_[0].position = VECTOR2(s * 3, s);
	sprite_[0].size = VECTOR2(s * 6, s);
	sprite_[0].enable = false;

	s *= 0.75f;
	float x = s * 1.77778f;
	sprite_[1].Init(systems_, 255, (int)Texture::Base::UNOWN);
	sprite_[1].position = VECTOR2(Half(x) + 10, s * 1.3f);
	sprite_[1].size = VECTOR2(x, s);
	sprite_[1].enable = false;

	return S_OK;
}

void Screenshot::Update(void)
{
	UIUpdate();
}

void Screenshot::Draw(void)
{
	// SHARE�{�^��
	if (systems_->GetInput()->GetCtrl(0)->Trigger(Input::GAMEPAD_SHARE, DIK_F2))
	{
		// �`�撆�ɂ�肽���͂Ȃ����A�����ł��Ȃ���
		// �A���ŎB�e�����ۂ�ScreenShot��UI�܂Ńe�N�X�`���ɏ������܂�Ă��܂�
		CreateTexture();
	}

	if (!enable_) { return; }
	systems_->GetRenderer()->GetWrapper()->DrawScreenshot(sprite_[0]);
	systems_->GetRenderer()->GetWrapper()->DrawScreenshot(sprite_[1]);
}

void Screenshot::CreateTexture(void)
{
	// ���ݎ��Ԃ̎擾
	time_t timer = time(NULL);
	tm     time;
	localtime_s(&time, &timer);
	// ���Ԃ��t�@�C�����ɂ���
	string filename = "screenshot/";
	char add[BUF_SIZE];
	sprintf_s(add, BUF_SIZE, "%d%02d%02d", 1900 + time.tm_year, 1 + time.tm_mon, time.tm_mday);
	filename += add;
	sprintf_s(add, BUF_SIZE, "_%02d%02d%02d.bmp", time.tm_hour, time.tm_min, time.tm_sec);
	filename += add;

	// �X�N���[���V���b�g�̍쐬
	systems_->GetRenderer()->GetWrapper()->CreateScreenshot(filename);
	// UI�`�掞��
	screenUICnt_ = 60 * 2;
}

void Screenshot::UIUpdate(void)
{
	// �펞��\���ɐݒ�(�`�掞�̂݉��ŕύX)
	enable_ = false;

	// UI�`��
	if (screenUICnt_ > 0)
	{
		// UI��`�悷��
		enable_ = true;

		screenUICnt_--;
		// ��莞�ԂŃe�N�X�`���̉��
		if (screenUICnt_ == 0) { systems_->GetRenderer()->GetWrapper()->ReleaseScreenshot(); }
	}
}
