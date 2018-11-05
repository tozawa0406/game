#include "Debug.h"
#include "../../Windows/Windows.h"
#include "../GameSystems.h"
#include "GuiManager.h"

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
Debug::Debug(Systems* systems) : Interface(systems), debug_(false), debugPause_(false), gui_(nullptr)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
Debug::~Debug(void)
{
}

/* @fn		Init
 * @brief	初期化
 * @param	なし
 * @return	初期化に成功したかどうかをHRESULTで返す		*/
HRESULT Debug::Init(void)
{
#ifdef _SELF_DEBUG
	// guiのマネージャーの生成
	gui_ = new GuiManager;
	if (gui_)
	{
		gui_->SetDebug(this);
		if (FAILED(gui_->Init()))
		{
			return E_FAIL;
		}
	}
#endif

	return S_OK; 
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void Debug::Uninit(void)
{
#ifdef _SELF_DEBUG
	UninitDeletePtr(gui_);
#endif
}

/* @fn		Update
 * @brief	更新処理
 * @param	なし
 * @return	なし					*/
void Debug::Update(void)
{
	// デバッグ時でなければ返る
	if (!debug_) { return; }

	if (gui_) { gui_->Update(); }
}

/* @fn		Draw
 * @brief	描画処理
 * @param	なし
 * @return	なし					*/
void Debug::Draw(void) 
{
	// デバッグ時でなければ返る
	if (!debug_) { return; }

	if (gui_) { gui_->Draw(); }
}

/* @fn		On
 * @brief	デバッグ機能のON/OFFスイッチ関数
 * @param	なし
 * @return	なし					*/
void Debug::On(void)
{
#ifdef _SELF_DEBUG
	//フラグの反転
	debug_ = !debug_;
#endif
}
