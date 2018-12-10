//-----------------------------------------------------------------------------
//
//	ベクトル[Vector.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using std::string;

//-----------------------------------------------------------------------------
//	構造体定義
//-----------------------------------------------------------------------------
struct VECTOR3;
struct VECTOR2
{
public:
	float x, y;

	// コンストラクタ
	VECTOR2(void);
	VECTOR2(float f);
	VECTOR2(float x, float y);

	// 演算子のオーバーロード
	VECTOR2& operator = (const VECTOR3& v);

	VECTOR2& operator += (const VECTOR2& v);
	VECTOR2& operator -= (const VECTOR2& v);
	VECTOR2& operator *= (float f);
	VECTOR2& operator /= (float f);

	VECTOR2 operator + ();
	VECTOR2 operator - ();

	VECTOR2 operator + (const VECTOR2& v) const;
	VECTOR2 operator - (const VECTOR2& v) const;
	VECTOR2 operator * (float f) const;
	VECTOR2 operator / (float f) const;

	bool operator == (const VECTOR2& v);
	bool operator != (float f);
	bool operator != (const VECTOR2& v);
};

struct VECTOR3
{
public:
	float x, y, z;

	// コンストラクタ
	VECTOR3(void);
	VECTOR3(float f);
	VECTOR3(const float x, const float y, const float z);

	// 演算子のオーバーロード
	VECTOR3& operator = (const VECTOR2& v);

	VECTOR3& operator += (const VECTOR3& v);
	VECTOR3& operator -= (const VECTOR3& v);
	VECTOR3& operator *= (float f);
	VECTOR3& operator /= (float f);

	VECTOR3& operator + (void);
	VECTOR3& operator - (void);

	VECTOR3 operator + (const VECTOR3& v) const;
	VECTOR3 operator - (const VECTOR3& v) const;
	VECTOR3 operator * (float f) const;
	VECTOR3 operator * (const VECTOR3& v) const;
	VECTOR3 operator / (float f) const;
	VECTOR3 operator / (const VECTOR3& v) const;

	bool operator == (const VECTOR3& v);
	bool operator != (float f);
	bool operator != (const VECTOR3& v);
	bool operator != (std::nullptr_t t)const;

	operator float*(void) { return &x; }
};

struct VECTOR4
{
public:
	float x, y, z, w;

	// コンストラクタ
	VECTOR4(void);
	VECTOR4(float f);
	VECTOR4(float x, float y, float z, float w);

	// 演算子のオーバーロード
	VECTOR4& operator += (const VECTOR4& v);
	VECTOR4& operator -= (const VECTOR4& v);
	VECTOR4& operator *= (float f);
	VECTOR4& operator /= (float f);

	VECTOR4& operator + (void);
	VECTOR4& operator - (void);
	VECTOR4 operator + (const VECTOR4& v) const;
	VECTOR4 operator - (const VECTOR4& v) const;
	VECTOR4 operator * (float f) const;
	VECTOR4 operator / (float f) const;

	bool operator == (const VECTOR4& v);
	bool operator != (const VECTOR4& v);
	bool operator != (std::nullptr_t t)const;
};

struct VECINT2
{
public:
	int x, y;

	// コンストラクタ
	VECINT2(void) {}
	VECINT2(int i) : x(i), y(i) {}
	VECINT2(int x, int y) : x(x), y(y) {}
};


//-----------------------------------------------------------------------------
//	関数定義
//-----------------------------------------------------------------------------
float VecDot(const VECTOR2 &r1, const VECTOR2 &r2);			// 内積(2次元)
float VecDot(const VECTOR3 &r1, const VECTOR3 &r2);			// 内積(3次元)
float   VecCross(const VECTOR2& r1, const VECTOR2& r2);		// 外積(2次元)
VECTOR3 VecCross(const VECTOR3& r1, const VECTOR3& r2);		// 外積(3次元)

float VecLengthSq(const VECTOR2& v);		// 長さの2乗(2次元)
float VecLength(const VECTOR2& v);			// 長さ(2次元)
float VecLengthSq(const VECTOR3& v);		// 長さの2乗(3次元)
float VecLength(const VECTOR3& v);			// 長さ(3次元)

VECTOR2 VecNorm(const VECTOR2& v);			// 単位ベクトル(2次元)
VECTOR3 VecNorm(const VECTOR3& v);			// 単位ベクトル(3次元)

VECTOR3 V3(const VECTOR2& v);			// 2次元ベクトルから3次元ベクトルへ(z = 0)
VECTOR2 V2(const VECTOR3& v);			// 3次元ベクトルから2次元ベクトルへ(z切り捨て)

VECTOR2 VecRestriction(const VECTOR2& v, float restriction);		// ベクトルの長さ制限(2次元)
VECTOR3 VecRestriction(const VECTOR3& v, float restriction);		// ベクトルの長さ制限(3次元)

VECTOR2 VecCircle(int range);			// 円周の範囲を指定するとランダムで速度が返る

#endif //_VECTOR_H_
