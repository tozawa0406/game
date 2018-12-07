#include "Screenshot.h"
#include "../Windows/Windows.h"
#include "GameSystems.h"
#include "Input/Controller.h"

//! @def	char�̃o�b�t�@�T�C�Y
static constexpr int BUF_SIZE = 15;

/* @brief	�R���X�g���N�^
 * @param	(systems)	�V�X�e���ւ̃|�C���^		*/
Screenshot::Screenshot(Systems* systems) : Interface(systems)
	, screenUICnt_(0)
	, renderTarget_(nullptr)
	, wrapper_(nullptr)
{
}

/* @brief	�f�X�g���N�^			*/
Screenshot::~Screenshot(void)
{
}

/* @brief	����������
 * @param	�Ȃ�
 * @return	�������s				*/
HRESULT Screenshot::Init(void)
{
	if (systems_)
	{
		if (const auto& graphics = systems_->GetRenderer())
		{
			renderTarget_	= graphics->GetRenderTarget();
			wrapper_		= graphics->GetWrapper();
		}
	}

	return S_OK;
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Screenshot::Update(void)
{
	// UI�`��̃J�E���g
	if (screenUICnt_ > 0)
	{
		screenUICnt_--;
	}
}

/* @brief	�`�揈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Screenshot::Draw(void)
{
	// SHARE�{�^��
	if (systems_->GetInput()->GetCtrl(0)->Trigger(Input::GAMEPAD_SHARE, DIK_F2))
	{
		// �`�撆�ɂ�肽���͂Ȃ����A�����ł��Ȃ���
		// �A���ŎB�e�����ۂ�ScreenShot��UI�܂Ńe�N�X�`���ɏ������܂�Ă��܂�
		CreateTexture();

		// UI�`�掞��
		screenUICnt_ = 60 * 2;
	}

	// ���ԓ��ł���Ε`��
	if (screenUICnt_ > 0)
	{
		// UI�̕`��
		float s = Graphics::HEIGHT * 0.1f;
		if (wrapper_)
		{
			wrapper_->SetTexture(0, static_cast<int>(Texture::Base::SSUI));
			wrapper_->DrawQuad(VECTOR2(s * 3, s), VECTOR2(s * 6, s));
		}

		// �X�N���[���V���b�g�̕`��
		if (renderTarget_)
		{
			s *= 0.75f;
			float x = s * 1.77778f;
			renderTarget_->Draw(RenderTarget::List::SCREEN_SHOT, VECTOR2(Half(x) + 10, s * 1.3f), VECTOR2(x, s));
		}
	}
}

/* @brief	�X�N���[���V���b�g�̍쐬
 * @sa		Draw()
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
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
	if (renderTarget_)
	{
		renderTarget_->CreateScreenshot(filename);
	}
}
