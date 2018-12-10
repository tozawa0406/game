/*
 * @file	Color.h
 * @brief	�F
 * @author	���V�đ�
 * @date	2018/10/14
 */
#ifndef _COLOR_H_
#define _COLOR_H_

typedef unsigned char byte;

struct COLOR
{
	//! �F(R, G, B, A)[0�`1]
	float r, g, b, a;

	/* @fn		�R���X�g���N�^		*/
	COLOR(void) {}

	/* @fn		�R���X�g���N�^
	 * @param	(f)		������S�Ă̐F�v�f��			*/
	COLOR(float f) : r(f), g(f), b(f), a(f) {}

	/* @fn		�R���X�g���N�^
	 * @param	(r, g, b)	�����ŐF�v�f���w��[a��1]	*/
	COLOR(float r, float g, float b) : r(r), g(g), b(b) {}

	/* @fn		�R���X�g���N�^
	 * @param	(r, g, b, a)	�����ŐF�v�f���w��		*/
	COLOR(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	/* @fn		operator float*
	 * @brief	�擪�A�h���X			*/
	operator float*(void) { return &r; }

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
	static COLOR RGBA(byte r, byte g, byte b, byte a)
	{
		float inv = 1.0f / 255;
		return COLOR(r * inv, g * inv, b * inv, a * inv);
	}
};

#endif // _COLOR_H_
