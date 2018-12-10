#ifndef _DX11_FONT_H_
#define _DX11_FONT_H_

#include "../../Utility/Font.h"
#include "../Dx11Include.h"

class Dx11Font : public Font
{
	struct BITMAPFONTDATA
	{
		TEXTMETRIC		TM;
		GLYPHMETRICS	GM;
		byte*			ptr;
	};
public:
	/* @brief	コンストラクタ	*/
	Dx11Font(void);
	/* @brief	デストラクタ	*/
	~Dx11Font(void);

	/* @brief	初期化処理
	 * @param	なし
	 * @return	成功失敗		*/
	HRESULT Init(Graphics* graphics) override;

	/* @brief	後処理
	 * @param	なし
	 * @return	なし			*/
	void	Uninit(void) override;

private:
	/* @brief	フォント生成
	 * @param	(font)		生成するフォント
	 * @param	(fontSize)	フォントのサイズ
	 * @return	成功失敗		*/
	HRESULT CreateFontData(const char* font, int fontSize) override;

	/* @brief	ビットマップのフォントデータ作成
	 * @param	(fontData)		テクスチャに書き込むフォントデータ
	 * @param	(font)			テクスチャに書き込むフォント
	 * @param	(fontSize)		生成するテクスチャのサイズ
	 * @return	成功失敗		*/
	HRESULT CreateBitMapFontData(BITMAPFONTDATA& fontData, const char* font, int fontSize);

	
	/* @brief	描画処理
	 * @param	(font)	描画文字列
	 * @return	なし			*/
	void DrawFont(const string& font, VECTOR2 position, VECTOR2 size, COLOR color) override;

	std::unordered_map<string, ID3D11ShaderResourceView*> font_;
};

#endif // _DX11_FONT_H_