/*
 * @file		CanvasRendererBase.h
 * @brief		UI描画の基底クラス
 * @author		戸澤翔太
 * @data		2018/12/10
 */
#ifndef _CANVAS_RENDERER_BASE_H_
#define _CANVAS_RENDERER_BASE_H_

#include "../RendererBase.h"

class CanvasRendererBase : public RendererBase
{
public:
	//! @enum	UI描画の種類
	enum class CanvasType : uint8
	{
		Image = 0,
		Text,

		MAX
	};

	/* @brief	コンストラクタ	*/
	CanvasRendererBase(CanvasType type) : RendererBase(RendererType::Canvas), position_(VECTOR2(0)), size_(VECTOR2(0)), color_(COLOR(1)), priority_(0), manager_(nullptr), CanvasType_(type) {}
	/* @brief	デストラクタ	*/
	virtual ~CanvasRendererBase(void) {}

	/* @brief	後処理			*/
	virtual void Uninit() = 0;

	/* @brief	位置取得(画面座標)		*/
	inline const VECTOR2&	GetPosition(void)		const { return position_;	}
	/* @brief	大きさ取得(画面座標)	*/
	inline const VECTOR2&	GetSize(void)			const { return size_;		}
	/* @brief	色の取得				*/
	inline const COLOR&		GetColor(void)			const { return color_;		}
	/* @brief	描画順取得				*/
	inline		 byte		GetPriority(void)		const { return priority_;	}
	/* @brief	種類の取得				*/
	inline		 CanvasType	GetCanvasType(void)		const { return CanvasType_;	}

	/* @brief	位置設定(画面座標)		*/
	inline void SetPosition(const VECTOR2& positoin)	{ position_ = positoin; }
	/* @brief	大きさ設定(画面座標)	*/
	inline void SetSize(const VECTOR2& size)			{ size_		= size;		}
	/* @brief	色の設定				*/
	inline void SetColor(const COLOR& color)			{ color_	= color;	}
	/* @brief	描画順変更				*/
	void		SetPriority(byte priority)				{ priority_ = priority; if (manager_) { manager_->Sort(); } }

protected:
	/* @brief	初期化(派生先から呼び出す)	*/
	void OnInit(void)
	{
		if (const auto& systems = Systems::Instance()) { if (const auto& manager = systems->GetCanvasRenderer()) { manager_ = manager; manager->Add(this); } }
	}
	/* @brief	後処理(派生先から呼び出す)	*/
	void OnUninit(void)
	{
		if (manager_) { manager_->Remove(this); }
	}

	VECTOR2		position_;		//! 位置(画面座標)
	VECTOR2		size_;			//! 大きさ(画面座標)
	COLOR		color_;			//! 色
	byte		priority_;		//! 描画順

	CanvasRendererManager* manager_;	//! マネージャーへのポインタ

private:
	CanvasType	CanvasType_;			//! 種類
};

#endif // _CANVAS_RENDERER_BASE_H_