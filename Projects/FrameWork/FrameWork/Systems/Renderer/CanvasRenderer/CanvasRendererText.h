/*
 * @file		CanvasRendererText.h
 * @brief		UIテキストクラス
 * @author		戸澤翔太
 * @data		2018/12/11
 */
#ifndef _CANVAS_RENDERER_TEXT_H_
#define _CANVAS_RENDERER_TEXT_H_

#include "CanvasRendererBase.h"

namespace CanvasRenderer
{
	class Text : public CanvasRendererBase
	{
	public:
		/* @brief	コンストラクタ		*/
		Text(void);
		/* @brief	デストラクタ		*/
		~Text(void);

		/* @brief	初期化
		 * @param	(priority)	描画順
		 * @param	(s)			生成する文字
		 * @param	(fontSize)	文字テクスチャの大きさ
		 * @return	なし				*/
		void Init(byte priority, const string& s, int fontSize);
		/* @brief	後処理				*/
		void Uninit(void) override;

		/* @brief	描画文字の取得		*/
		inline const string&	GetString(void)		{ return string_;	}
		/* @brief	描画領域の取得		*/
		inline const VECTOR2&	GetDrawSize(void)	{ return drawSize_; }
		/* @brief	折り返し表示の有無	*/
		inline		 bool		GetFaldBack(void)	{ return faldBack_; }

		/* @brief	描画文字の設定		*/
		inline void SetString(const string& s)			{ string_ = s;			}
		/* @brief	描画領域の設定		*/
		inline void SetDrawSize(const VECTOR2& size)	{ drawSize_ = size;		}
		/* @brief	折り返し表示の有無	*/
		inline void SetFaldBack(bool faldBack)			{ faldBack_ = faldBack; }

	private:
		string		string_;		//! 描画文字
		VECTOR2		drawSize_;		//! 描画領域
		bool		faldBack_;		//! 折り返して表示
	};
}

#endif // _CANVAS_RENDERER_TEXT_H_