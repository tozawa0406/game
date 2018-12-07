#include "Screenshot.h"
#include "../Windows/Windows.h"
#include "GameSystems.h"
#include "Input/Controller.h"

//! @def	charのバッファサイズ
static constexpr int BUF_SIZE = 15;

/* @brief	コンストラクタ
 * @param	(systems)	システムへのポインタ		*/
Screenshot::Screenshot(Systems* systems) : Interface(systems)
	, screenUICnt_(0)
	, renderTarget_(nullptr)
	, wrapper_(nullptr)
{
}

/* @brief	デストラクタ			*/
Screenshot::~Screenshot(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	成功失敗				*/
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

/* @brief	更新処理
 * @param	なし
 * @return	なし					*/
void Screenshot::Update(void)
{
	// UI描画のカウント
	if (screenUICnt_ > 0)
	{
		screenUICnt_--;
	}
}

/* @brief	描画処理
 * @param	なし
 * @return	なし					*/
void Screenshot::Draw(void)
{
	// SHAREボタン
	if (systems_->GetInput()->GetCtrl(0)->Trigger(Input::GAMEPAD_SHARE, DIK_F2))
	{
		// 描画中にやりたくはないが、ここでやらないと
		// 連続で撮影した際にScreenShotのUIまでテクスチャに書き込まれてしまう
		CreateTexture();

		// UI描画時間
		screenUICnt_ = 60 * 2;
	}

	// 時間内であれば描画
	if (screenUICnt_ > 0)
	{
		// UIの描画
		float s = Graphics::HEIGHT * 0.1f;
		if (wrapper_)
		{
			wrapper_->SetTexture(0, static_cast<int>(Texture::Base::SSUI));
			wrapper_->DrawQuad(VECTOR2(s * 3, s), VECTOR2(s * 6, s));
		}

		// スクリーンショットの描画
		if (renderTarget_)
		{
			s *= 0.75f;
			float x = s * 1.77778f;
			renderTarget_->Draw(RenderTarget::List::SCREEN_SHOT, VECTOR2(Half(x) + 10, s * 1.3f), VECTOR2(x, s));
		}
	}
}

/* @brief	スクリーンショットの作成
 * @sa		Draw()
 * @param	なし
 * @return	なし					*/
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
	if (renderTarget_)
	{
		renderTarget_->CreateScreenshot(filename);
	}
}
