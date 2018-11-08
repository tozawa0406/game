/*
 * @file	Color.h
 * @brief	�F
 * @author	���V�đ�
 * @date	2018/10/14
 */
#ifndef _COLOR_H_
#define _COLOR_H_

//-----------------------------------------------------------------------------
//	�\���̒�`
//-----------------------------------------------------------------------------
struct COLOR
{
	//! �F(R, G, B, A)[0�`1]
	float r, g, b, a;

	/* @fn		�R���X�g���N�^		*/
	COLOR(void) {}

	/* @fn		�R���X�g���N�^
	 * @param	(f)		������S�Ă̐F�v�f��			*/
	COLOR(float f)
	{
		this->r = f;
		this->g = f;
		this->b = f;
		this->a = f;
	}

	/* @fn		�R���X�g���N�^
	 * @param	(r, g, b)	�����ŐF�v�f���w��[a��1]	*/
	COLOR(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 1;
	}

	/* @fn		�R���X�g���N�^
	 * @param	(r, g, b, a)	�����ŐF�v�f���w��		*/
	COLOR(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	/* @fn		operator float*
	 * @brief	�擪�A�h���X			*/
	operator float*(void)
	{
		return &r;
	}

	/* @fn		operator =
	 * @brief	�l�̃R�s�[				*/
	COLOR operator = (COLOR c)
	{
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->a = c.a;
		return *this;
	}

	/* @fn		operator == 
	 * @brief	��r�֐�				*/
	bool operator == (const COLOR& c)
	{
		return ((this->r == c.r && this->g == c.g) && (this->b == c.b && this->a == c.a)) ? true : false;
	}


	/* @fn		RBGA
	 * @brief	0�`255�ŐF�v�f���w�肷��		*/
	static COLOR RGBA(BYTE r, BYTE g, BYTE b, BYTE a)
	{
		float inv = 1.0f / 255;
		return COLOR(r * inv, g * inv, b * inv, a * inv);
	}
};

#endif // _COLOR_H_
