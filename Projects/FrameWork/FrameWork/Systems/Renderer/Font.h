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
	/* @brief	�R���X�g���N�^	*/
	Font(void);
	/* @brief	�f�X�g���N�^	*/
	~Font(void);

	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�������s		*/
	HRESULT Init(DirectX11* directX11);

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�			*/
	void	Uninit(void);

	/* @brief	�t�H���g����
	 * @param	(fonts)		��������t�H���g
	 * @param	(fontSize)	�t�H���g�̃T�C�Y
	 * @return	�������s		*/
	HRESULT CreateStringData(const string& fonts, int fontSize);

	/* @brief	�`�揈��
	 * @param	(font)	�`�敶����
	 * @return	�Ȃ�			*/
	void DrawFont(const string& font, VECTOR2 position, VECTOR2 size, COLOR color);

	void Draw(const string& fonts, VECTOR2 position, VECTOR2 size, COLOR color);

private:
	/* @brief	�t�H���g����
	 * @param	(font)		��������t�H���g
	 * @param	(fontSize)	�t�H���g�̃T�C�Y
	 * @return	�������s		*/
	HRESULT CreateFontData(const TCHAR* font, int fontSize);

	HRESULT CreateBitMapFontData(BITMAPFONTDATA& fontData, const TCHAR* font, int fontSize);

	std::unordered_map<string, ID3D11ShaderResourceView*> font_;
	DirectX11* directX11_;
};

#endif // _FONT_H_