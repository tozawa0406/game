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
	// スクリーンショット用UI生成
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
	// SHAREボタン
	if (systems_->GetInput()->GetCtrl(0)->Trigger(Input::GAMEPAD_SHARE, DIK_F2))
	{
		// 描画中にやりたくはないが、ここでやらないと
		// 連続で撮影した際にScreenShotのUIまでテクスチャに書き込まれてしまう
		CreateTexture();
	}

	if (!enable_) { return; }
	systems_->GetRenderer()->GetWrapper()->DrawScreenshot(sprite_[0]);
	systems_->GetRenderer()->GetWrapper()->DrawScreenshot(sprite_[1]);
}

void Screenshot::CreateTexture(void)
{
	// 現在時間の取得
	time_t timer = time(NULL);
	tm     time;
	localtime_s(&time, &timer);
	// 時間をファイル名にする
	string filename = "screenshot/";
	char add[BUF_SIZE];
	sprintf_s(add, BUF_SIZE, "%d%02d%02d", 1900 + time.tm_year, 1 + time.tm_mon, time.tm_mday);
	filename += add;
	sprintf_s(add, BUF_SIZE, "_%02d%02d%02d.bmp", time.tm_hour, time.tm_min, time.tm_sec);
	filename += add;

	// スクリーンショットの作成
	systems_->GetRenderer()->GetWrapper()->CreateScreenshot(filename);
	// UI描画時間
	screenUICnt_ = 60 * 2;
}

void Screenshot::UIUpdate(void)
{
	// 常時非表示に設定(描画時のみ下で変更)
	enable_ = false;

	// UI描画
	if (screenUICnt_ > 0)
	{
		// UIを描画する
		enable_ = true;

		screenUICnt_--;
		// 一定時間でテクスチャの解放
		if (screenUICnt_ == 0) { systems_->GetRenderer()->GetWrapper()->ReleaseScreenshot(); }
	}
}
