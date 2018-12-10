/*
 * @file		CanvasRendererText.h
 * @brief		UI�e�L�X�g�N���X
 * @author		���V�đ�
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
		/* @brief	�R���X�g���N�^		*/
		Text(void);
		/* @brief	�f�X�g���N�^		*/
		~Text(void);

		/* @brief	������
		 * @param	(priority)	�`�揇
		 * @param	(s)			�������镶��
		 * @param	(fontSize)	�����e�N�X�`���̑傫��
		 * @return	�Ȃ�				*/
		void Init(byte priority, const string& s, int fontSize);
		/* @brief	�㏈��				*/
		void Uninit(void) override;

		/* @brief	�`�敶���̎擾		*/
		inline const string&	GetString(void)		{ return string_;	}
		/* @brief	�`��̈�̎擾		*/
		inline const VECTOR2&	GetDrawSize(void)	{ return drawSize_; }
		/* @brief	�܂�Ԃ��\���̗L��	*/
		inline		 bool		GetFaldBack(void)	{ return faldBack_; }

		/* @brief	�`�敶���̐ݒ�		*/
		inline void SetString(const string& s)			{ string_ = s;			}
		/* @brief	�`��̈�̐ݒ�		*/
		inline void SetDrawSize(const VECTOR2& size)	{ drawSize_ = size;		}
		/* @brief	�܂�Ԃ��\���̗L��	*/
		inline void SetFaldBack(bool faldBack)			{ faldBack_ = faldBack; }

	private:
		string		string_;		//! �`�敶��
		VECTOR2		drawSize_;		//! �`��̈�
		bool		faldBack_;		//! �܂�Ԃ��ĕ\��
	};
}

#endif // _CANVAS_RENDERER_TEXT_H_