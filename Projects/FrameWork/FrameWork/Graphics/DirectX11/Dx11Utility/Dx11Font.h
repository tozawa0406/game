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
	/* @brief	�R���X�g���N�^	*/
	Dx11Font(void);
	/* @brief	�f�X�g���N�^	*/
	~Dx11Font(void);

	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�������s		*/
	HRESULT Init(Graphics* graphics) override;

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�			*/
	void	Uninit(void) override;

private:
	/* @brief	�t�H���g����
	 * @param	(font)		��������t�H���g
	 * @param	(fontSize)	�t�H���g�̃T�C�Y
	 * @return	�������s		*/
	HRESULT CreateFontData(const char* font, int fontSize) override;

	/* @brief	�r�b�g�}�b�v�̃t�H���g�f�[�^�쐬
	 * @param	(fontData)		�e�N�X�`���ɏ������ރt�H���g�f�[�^
	 * @param	(font)			�e�N�X�`���ɏ������ރt�H���g
	 * @param	(fontSize)		��������e�N�X�`���̃T�C�Y
	 * @return	�������s		*/
	HRESULT CreateBitMapFontData(BITMAPFONTDATA& fontData, const char* font, int fontSize);

	
	/* @brief	�`�揈��
	 * @param	(font)	�`�敶����
	 * @return	�Ȃ�			*/
	void DrawFont(const string& font, VECTOR2 position, VECTOR2 size, COLOR color) override;

	std::unordered_map<string, ID3D11ShaderResourceView*> font_;
};

#endif // _DX11_FONT_H_