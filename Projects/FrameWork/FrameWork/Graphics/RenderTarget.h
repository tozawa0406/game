/*
 * @file		RenderTarget.h
 * @brief		グラフィックスAPI毎のレンダーターゲットの処理
 * @author		戸澤翔太
 * @data		2018/12/21
 */

#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include "../Define/Define.h"

class RenderTarget
{
public:
	/* @enum	レンダーターゲットの種類	*/
	enum class List : uint8
	{
		DEFAULT = 0,
		SCREEN_SHOT,
		COLOR,
		POSITION,
		NORMAL,
		MAX
	};

	//! @def	描画の大きさ横
	static constexpr float SIZE_X = 240;
	//! @def	描画の大きさ縦
	static constexpr float SIZE_Y = 135;

	/* @brief	コンストラクタ		*/
	RenderTarget(void) : debugDraw_(List::MAX) {}
	/* @brief	デストラクタ		*/
	virtual ~RenderTarget(void) {}

	/* @brief	初期化処理			*/
	virtual HRESULT Init(void)		= 0;
	/* @brief	後処理				*/
	virtual void	Uninit(void)	= 0;

	/* @brief	レンダーターゲットのクリア処理			*/
	virtual void ClearRendererTarget(List num, COLOR color) = 0;

	/* @brief	影の描画開始		*/
	virtual void BeginDrawShadow(int i) = 0;
	/* @brief	影の描画終了		*/
	virtual void EndDrawShadow(void) = 0;

	/* @brief	マルチレンダーターゲットの描画開始		*/
	virtual void BeginMultiRendererTarget(void) = 0;
	/* @brief	マルチレンダーターゲットの描画終了		*/
	virtual void EndMultiRendererTarget(void)	= 0;

	/* @brief	Guiの更新処理		*/
	virtual void GuiUpdate(void) {}

	/* @brief	デバッグ用描画処理	*/
	virtual void Draw(List num, VECTOR2 position, VECTOR2 size) = 0;
	/* @brief	デバッグ用描画処理	*/
	virtual void DrawShadowMap(void) = 0;

	/* @brief	スクリーンショットの生成			*/
	virtual void CreateScreenshot(const string& filename) = 0;

	/* @brief	デバッグ用に描画するレンダーターゲットの設定
	 * @param	(num)	描画したいレンダーターゲットの種類
	 * @return	なし				*/
	inline void SetDebugDraw(List num) { debugDraw_ = num; }

protected:
	//! デバッグで画面に描画するレンダーターゲットの種類
	List debugDraw_;
};

#endif // _RENDER_TARGET_H_