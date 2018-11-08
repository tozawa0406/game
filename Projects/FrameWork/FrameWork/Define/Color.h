/*
 * @file	Color.h
 * @brief	色
 * @author	戸澤翔太
 * @date	2018/10/14
 */
#ifndef _COLOR_H_
#define _COLOR_H_

//-----------------------------------------------------------------------------
//	構造体定義
//-----------------------------------------------------------------------------
struct COLOR
{
	//! 色(R, G, B, A)[0～1]
	float r, g, b, a;

	/* @fn		コンストラクタ		*/
	COLOR(void) {}

	/* @fn		コンストラクタ
	 * @param	(f)		引数を全ての色要素に			*/
	COLOR(float f)
	{
		this->r = f;
		this->g = f;
		this->b = f;
		this->a = f;
	}

	/* @fn		コンストラクタ
	 * @param	(r, g, b)	引数で色要素を指定[aは1]	*/
	COLOR(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 1;
	}

	/* @fn		コンストラクタ
	 * @param	(r, g, b, a)	引数で色要素を指定		*/
	COLOR(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	/* @fn		operator float*
	 * @brief	先頭アドレス			*/
	operator float*(void)
	{
		return &r;
	}

	/* @fn		operator =
	 * @brief	値のコピー				*/
	COLOR operator = (COLOR c)
	{
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->a = c.a;
		return *this;
	}

	/* @fn		operator == 
	 * @brief	比較関数				*/
	bool operator == (const COLOR& c)
	{
		return ((this->r == c.r && this->g == c.g) && (this->b == c.b && this->a == c.a)) ? true : false;
	}


	/* @fn		RBGA
	 * @brief	0～255で色要素を指定する		*/
	static COLOR RGBA(BYTE r, BYTE g, BYTE b, BYTE a)
	{
		float inv = 1.0f / 255;
		return COLOR(r * inv, g * inv, b * inv, a * inv);
	}
};

#endif // _COLOR_H_
