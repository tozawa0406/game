#ifndef _FONT_H_
#define _FONT_H_

#include "../../Define/Define.h"
#include "../../Windows/Windows.h"
#include "../../Graphics/Graphics.h"
#include <unordered_map>

class Font
{
public:
	/* @brief	�R���X�g���N�^	*/
	Font(void);
	/* @brief	�f�X�g���N�^	*/
	virtual ~Font(void);

	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�������s		*/
	virtual HRESULT Init(Graphics* graphics) = 0;

	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�			*/
	virtual void	Uninit(void) = 0;

	/* @brief	�t�H���g����
	 * @param	(fonts)		��������t�H���g(������)
	 * @param	(fontSize)	�t�H���g�̃T�C�Y
	 * @return	�������s		*/
	HRESULT CreateStringData(const string& fonts, int fontSize);

	/* @brief	�`�揈��
	 * @param	(fonts)		�`�悷�镶����
	 * @param	(positoin)	�`��ʒu(������W)
	 * @param	(size)		�����T�C�Y
	 * @param	(color)		�����F
	 * @param	(faldBack)	�܂�Ԃ��`��
	 * @param	(allSize)	�`��̈�
	 * @return	�Ȃ�			*/
	void Draw(const string& fonts, VECTOR2 position, VECTOR2 size, COLOR color, bool faldBack = false, VECTOR2 allSize = VECTOR2(0, 0));

protected:
	/* @brief	�t�H���g����
	 * @param	(font)		��������t�H���g
	 * @param	(fontSize)	�t�H���g�̃T�C�Y
	 * @return	�������s		*/
	virtual HRESULT CreateFontData(const char* font, int fontSize) = 0;
	
	/* @brief	�`�揈��
	 * @param	(font)	�`�敶����
	 * @return	�Ȃ�			*/
	virtual void DrawFont(const string& font, VECTOR2 position, VECTOR2 size, COLOR color) = 0;

	Graphics* graphics_;
};

#endif // _FONT_H_