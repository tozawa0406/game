#ifndef _FONT_H_
#define _FONT_H_

#include "../../Define/Define.h"
#include "../../Windows/Windows.h"
#include "../../Graphics/Graphics.h"
#include <unordered_map>

class Font
{
public:
	/* @brief	コンストラクタ	*/
	Font(void);
	/* @brief	デストラクタ	*/
	virtual ~Font(void);

	/* @brief	初期化処理
	 * @param	なし
	 * @return	成功失敗		*/
	virtual HRESULT Init(Graphics* graphics) = 0;

	/* @brief	後処理
	 * @param	なし
	 * @return	なし			*/
	virtual void	Uninit(void) = 0;

	/* @brief	フォント生成
	 * @param	(fonts)		生成するフォント(文字列)
	 * @param	(fontSize)	フォントのサイズ
	 * @return	成功失敗		*/
	HRESULT CreateStringData(const string& fonts, int fontSize);

	/* @brief	描画処理
	 * @param	(fonts)		描画する文字列
	 * @param	(positoin)	描画位置(左上座標)
	 * @param	(size)		文字サイズ
	 * @param	(color)		文字色
	 * @param	(faldBack)	折り返し描画
	 * @param	(allSize)	描画領域
	 * @return	なし			*/
	void Draw(const string& fonts, VECTOR2 position, VECTOR2 size, COLOR color, bool faldBack = false, VECTOR2 allSize = VECTOR2(0, 0));

protected:
	/* @brief	フォント生成
	 * @param	(font)		生成するフォント
	 * @param	(fontSize)	フォントのサイズ
	 * @return	成功失敗		*/
	virtual HRESULT CreateFontData(const char* font, int fontSize) = 0;
	
	/* @brief	描画処理
	 * @param	(font)	描画文字列
	 * @return	なし			*/
	virtual void DrawFont(const string& font, VECTOR2 position, VECTOR2 size, COLOR color) = 0;

	Graphics* graphics_;
};

#endif // _FONT_H_