#ifndef _FONT_H_
#define _FONT_H_

#include "../../Define/Define.h"
#include "../../Windows/Windows.h"
#include <FrameWork/Graphics/DirectX11/Dx11Include.h>
#include <unordered_map>

class DirectX11;
class Font
{
	struct BITMAPFONTDATA
	{
		TEXTMETRIC		TM;
		GLYPHMETRICS	GM;
		BYTE*			ptr;
	};
public:
	/* @brief	コンストラクタ	*/
	Font(void);
	/* @brief	デストラクタ	*/
	~Font(void);

	/* @brief	初期化処理
	 * @param	なし
	 * @return	成功失敗		*/
	HRESULT Init(DirectX11* directX11);

	/* @brief	後処理
	 * @param	なし
	 * @return	なし			*/
	void	Uninit(void);

	/* @brief	フォント生成
	 * @param	(fonts)		生成するフォント
	 * @param	(fontSize)	フォントのサイズ
	 * @return	成功失敗		*/
	HRESULT CreateStringData(const string& fonts, int fontSize);

	/* @brief	描画処理
	 * @param	(font)	描画文字列
	 * @return	なし			*/
	void DrawFont(const string& font, VECTOR2 position, VECTOR2 size, COLOR color);

	void Draw(const string& fonts, VECTOR2 position, VECTOR2 size, COLOR color);

private:
	/* @brief	フォント生成
	 * @param	(font)		生成するフォント
	 * @param	(fontSize)	フォントのサイズ
	 * @return	成功失敗		*/
	HRESULT CreateFontData(const TCHAR* font, int fontSize);

	HRESULT CreateBitMapFontData(BITMAPFONTDATA& fontData, const TCHAR* font, int fontSize);

	std::unordered_map<string, ID3D11ShaderResourceView*> font_;
	DirectX11* directX11_;
};

#endif // _FONT_H_