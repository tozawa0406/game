/*
 * @file	Color.h
 * @brief	色
 * @author	戸澤翔太
 * @date	2018/10/14
 */
#ifndef _COLOR_H_
#define _COLOR_H_

typedef unsigned char byte;

struct COLOR
{
	//! 色(R, G, B, A)[0～1]
	float r, g, b, a;

	/* @brief	コンストラクタ		*/
	COLOR(void) : r(1), g(1), b(1), a(1) {}

	/* @brief	コンストラクタ
	 * @param	(f)		引数を全ての色要素に			*/
	COLOR(float f) : r(f), g(f), b(f), a(f) {}

	/* @brief	コンストラクタ
	 * @param	(r, g, b)	引数で色要素を指定[aは1]	*/
	COLOR(float r, float g, float b) : r(r), g(g), b(b), a(1) {}

	/* @brief	コンストラクタ
	 * @param	(r, g, b, a)	引数で色要素を指定		*/
	COLOR(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	/* @brief	先頭アドレス			*/
	operator float*(void) { return &r; }

	/* @brief	値のコピー				*/
	COLOR operator = (COLOR c)
	{
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->a = c.a;
		return *this;
	}

	/* @brief	比較関数				*/
	bool operator == (const COLOR& c)
	{
		return ((this->r == c.r && this->g == c.g) && (this->b == c.b && this->a == c.a)) ? true : false;
	}


	/* @brief	0～255で色要素を指定する		*/
	static COLOR RGBA(byte r, byte g, byte b, byte a)
	{
		float inv = 1.0f / 255;
		return COLOR(r * inv, g * inv, b * inv, a * inv);
	}

	static COLOR RGBA(byte r, byte g, byte b)
	{
		float inv = 1.0f / 255;
		return COLOR(r * inv, g * inv, b * inv, 1);
	}
};

#endif // _COLOR_H_
