/*
 * @file	Vector.h
 * @brief	ベクトル
 * @author	戸澤翔太
 * @date	2018/08/18
 */
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using std::string;

struct VECTOR3;
struct VECTOR2
{
public:
	float x, y;

	/* @brief	コンストラクタ		*/
	VECTOR2(void);

	/* @brief	コンストラクタ
	 * @param	(f)		初期化値	*/
	VECTOR2(float f);
	
	/* @brief	コンストラクタ
	 * @param	(x)		xの初期化値
	 * @param	(y)		yの初期化値	*/
	VECTOR2(float x, float y) :x(x), y(y) {}

	template<class T1, class T2>
	VECTOR2(const T1 x, const T2 y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}

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

	/* @brief	コンストラクタ		*/
	VECTOR3(void);

	/* @brief	コンストラクタ
	 * @param	(f)		初期化値	*/
	VECTOR3(float f);

	/* @brief	コンストラクタ
	 * @param	(x)		xの初期化値
	 * @param	(y)		yの初期化値
	 * @param	(z)		zの初期化値	*/
	VECTOR3(const float x, const float y, const float z);

	template<class T1, class T2, class T3>
	VECTOR3(const T1 x, const T2 y, const T3 z) : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z)) {}

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

	/* @brief	コンストラクタ		*/
	VECTOR4(void);

	/* @brief	コンストラクタ
	 * @param	(f)		初期化値	*/
	VECTOR4(float f);
	
	/* @brief	コンストラクタ
	 * @param	(x)		xの初期化値
	 * @param	(y)		yの初期化値
	 * @param	(z)		zの初期化値
	 * @param	(w)		wの初期化値	*/
	VECTOR4(float x, float y, float z, float w);

	template<class T1, class T2, class T3, class T4>
	VECTOR4(const T1 x, const T2 y, const T3 z, const T4 w) : x(static_cast<float>(x)), y(static_cast<float>(y))
															, z(static_cast<float>(z)), w(static_cast<float>(w)) {}

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

	/* @brief	コンストラクタ		*/
	VECINT2(void);

	/* @brief	コンストラクタ
	 * @param	(i)		初期化値	*/
	VECINT2(int i);

	/* @brief	コンストラクタ
	 * @param	(x)		xの初期化値
	 * @param	(y)		yの初期化値	*/
	VECINT2(int x, int y);
};

/* @brief	2次元の内積
 * @param	(r1)	ベクトル1
 * @param	(r2)	ベクトル2
 * @return	内積					*/
float VecDot(const VECTOR2 &r1, const VECTOR2 &r2);

/* @brief	3次元の内積
 * @param	(r1)	ベクトル1
 * @param	(r2)	ベクトル2
 * @return	内積					*/
float VecDot(const VECTOR3 &r1, const VECTOR3 &r2);

/* @brief	2次元の外積
 * @param	(r1)	ベクトル1
 * @param	(r2)	ベクトル2
 * @return	外積					*/
float   VecCross(const VECTOR2& r1, const VECTOR2& r2);

/* @brief	3次元の外積
 * @param	(r1)	ベクトル1
 * @param	(r2)	ベクトル2
 * @return	外積					*/
VECTOR3 VecCross(const VECTOR3& r1, const VECTOR3& r2);



/* @brief	2次元の長さの2乗
 * @param	(v)		ベクトル
 * @return	長さの2乗				*/
float VecLengthSq(const VECTOR2& v);

/* @brief	2次元の長さ
 * @param	(v)		ベクトル
 * @return	長さ					*/
float VecLength(const VECTOR2& v);

/* @brief	3次元の長さの2乗
 * @param	(v)		ベクトル
 * @return	長さの2乗				*/
float VecLengthSq(const VECTOR3& v);

/* @brief	3次元の長さ
 * @param	(v)		ベクトル
 * @return	長さ					*/
float VecLength(const VECTOR3& v);


/* @brief	2次元の単位ベクトル
 * @param	(v)		ベクトル
 * @return	単位ベクトル			*/
VECTOR2 VecNorm(const VECTOR2& v);

/* @brief	3次元の単位ベクトル
 * @param	(v)		ベクトル
 * @return	単位ベクトル			*/
VECTOR3 VecNorm(const VECTOR3& v);


/* @brief	2次元から3次元へ
 * @param	(v)		ベクトル
 * @return	3次元ベクトル(z = 0)		*/
VECTOR3 V3(const VECTOR2& v);

/* @brief	3次元から2次元へ
 * @param	(v)		ベクトル
 * @return	2次元ベクトル(z切り捨て)	*/
VECTOR2 V2(const VECTOR3& v);

/* @brief	2次元のベクトル長さ制限
 * @param	(v)				ベクトル
 * @param	(restriction)	制限値
 * @return	制限されたベクトル			*/
VECTOR2 VecRestriction(const VECTOR2& v, float restriction);

/* @brief	3次元のベクトル長さ制限
 * @param	(v)				ベクトル
 * @param	(restriction)	制限値
 * @return	制限されたベクトル			*/
VECTOR3 VecRestriction(const VECTOR3& v, float restriction);

/* @brief	円周の範囲を指定するとランダムで速度が返る
 * @param	(range)		範囲
 * @return	速度						*/
VECTOR2 VecCircle(int range);

#endif //_VECTOR_H_
