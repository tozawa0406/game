//-----------------------------------------------------------------------------
//
//	ベクトル[Vector.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Vector.h"

// コンストラクタ
VECTOR2::VECTOR2(void) {}

VECTOR2::VECTOR2(float f)
{
	this->x = f;
	this->y = f;
}

VECTOR2::VECTOR2(float x, float y)
{
	this->x = x;
	this->y = y;
}

// 演算子のオーバーロード
VECTOR2& VECTOR2::operator = (const VECTOR3& v)
{
	this->x = v.x;
	this->y = v.y;
	return *this;
}

VECTOR2& VECTOR2::operator += (const VECTOR2& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

VECTOR2& VECTOR2::operator -= (const VECTOR2& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

VECTOR2& VECTOR2::operator *= (float f)
{
	this->x *= f;
	this->y *= f;
	return *this;
}

VECTOR2& VECTOR2::operator /= (float f)
{
	float inv = 1.0f / f;
	this->x *= inv;
	this->y *= inv;
	return *this;
}

VECTOR2 VECTOR2::operator + (void)
{
	(this->x < 0) ? this->x *= -1 : this->x *= 1;
	(this->y < 0) ? this->y *= -1 : this->y *= 1;
	return *this;
}

VECTOR2 VECTOR2::operator - (void)
{
	(this->x > 0) ? this->x *= -1 : this->x *= 1;
	(this->y > 0) ? this->y *= -1 : this->y *= 1;
	return *this;
}

VECTOR2 VECTOR2::operator + (const VECTOR2& v) const
{
	return VECTOR2(x + v.x, y + v.y);
}

VECTOR2 VECTOR2::operator - (const VECTOR2& v) const
{
	return VECTOR2(x - v.x, y - v.y);
}

VECTOR2 VECTOR2::operator * (float f) const 
{
	return VECTOR2(x * f, y * f);
}

VECTOR2 VECTOR2::operator / (float f) const
{
	float inv = 1.0f / f;
	return VECTOR2(x * inv, y * inv);
}

bool VECTOR2::operator == (const VECTOR2& v)
{
	return (this->x == v.x && this->y == v.y) ? true : false;
}

bool VECTOR2::operator != (float f)
{
	return (this->x != f || this->y != f) ? true : false;
}

bool VECTOR2::operator != (const VECTOR2& v)
{
	return (this->x != v.x || this->y != v.y) ? true : false;
}



// コンストラクタ
VECTOR3::VECTOR3(void) {}

VECTOR3::VECTOR3(float f)
{
	this->x = f;
	this->y = f;
	this->z = f;
}

VECTOR3::VECTOR3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

VECTOR3& VECTOR3::operator = (const VECTOR2& v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = 0;
	return *this;
}

VECTOR3& VECTOR3::operator += (const VECTOR3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

VECTOR3& VECTOR3::operator -= (const VECTOR3& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

VECTOR3& VECTOR3::operator *= (float f)
{
	this->x *= f;
	this->y *= f;
	this->z *= f;
	return *this;
}

VECTOR3& VECTOR3::operator /= (float f)
{
	float inv = 1.0f / f;
	this->x *= inv;
	this->y *= inv;
	this->z *= inv;
	return *this;
}

VECTOR3& VECTOR3::operator + (void)
{
	(this->x < 0) ? this->x *= -1 : this->x *= 1;
	(this->y < 0) ? this->y *= -1 : this->y *= 1;
	(this->z < 0) ? this->z *= -1 : this->z *= 1;
	return *this;
}

VECTOR3& VECTOR3::operator - (void)
{
	*this *= -1;
	return *this;
}

VECTOR3 VECTOR3::operator + (const VECTOR3& v) const
{
	return VECTOR3(this->x + v.x, this->y + v.y, this->z + v.z);
}

VECTOR3 VECTOR3::operator - (const VECTOR3& v) const
{
	return VECTOR3(this->x - v.x, this->y - v.y, this->z - v.z);
}

VECTOR3 VECTOR3::operator * (float f) const
{
	return VECTOR3(this->x * f, this->y * f, this->z * f);
}

VECTOR3 VECTOR3::operator * (const VECTOR3& v) const
{
	return VECTOR3(this->x * v.x, this->y * v.y, this->z * v.z);
}

VECTOR3 VECTOR3::operator / (float f) const
{
	float inv = 1.0f / f;
	return VECTOR3(this->x * inv, this->y * inv, this->z * inv);
}

VECTOR3 VECTOR3::operator / (const VECTOR3& v) const
{
	return VECTOR3(this->x / v.x, this->y / v.y, this->z / v.z);
}

bool VECTOR3::operator == (const VECTOR3& v)
{
	return (this->x == v.x && this->y == v.y && this->z == v.z) ? true : false;
}

bool VECTOR3::operator != (float f)
{
	return (this->x != f || this->y != f || this->z != f) ? true : false;
}

bool VECTOR3::operator != (const VECTOR3& v)
{
	return (this->x != v.x || this->y != v.y || this->z != v.z) ? true : false;
}

bool VECTOR3::operator != (std::nullptr_t t)const
{
	return (this != t) ? true : false;
}



// コンストラクタ
VECTOR4::VECTOR4(void) {};

VECTOR4::VECTOR4(float f)
{
	this->x = f;
	this->y = f;
	this->z = f;
	this->w = f;
}

VECTOR4::VECTOR4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

// 演算子のオーバーロード
VECTOR4& VECTOR4::operator += (const VECTOR4& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;
	return *this;
}

VECTOR4& VECTOR4::operator -= (const VECTOR4& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;
	return *this;
}

VECTOR4& VECTOR4::operator *= (float f)
{
	this->x *= f;
	this->y *= f;
	this->z *= f;
	this->w *= f;
	return *this;
}

VECTOR4& VECTOR4::operator /= (float f)
{
	float inv = 1.0f / f;
	this->x *= inv;
	this->y *= inv;
	this->z *= inv;
	this->w *= inv;
	return *this;
}

VECTOR4& VECTOR4::operator + (void)
{
	(this->x < 0) ? this->x *= -1 : this->x *= 1;
	(this->y < 0) ? this->y *= -1 : this->y *= 1;
	(this->z < 0) ? this->z *= -1 : this->z *= 1;
	(this->w < 0) ? this->w *= -1 : this->w *= 1;
	return *this;
}

VECTOR4& VECTOR4::operator - (void)
{
	(this->x > 0) ? this->x *= -1 : this->x *= 1;
	(this->y > 0) ? this->y *= -1 : this->y *= 1;
	(this->z > 0) ? this->z *= -1 : this->z *= 1;
	(this->w > 0) ? this->w *= -1 : this->w *= 1;
	return *this;
}

VECTOR4 VECTOR4::operator + (const VECTOR4& v) const
{
	return VECTOR4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
}

VECTOR4 VECTOR4::operator - (const VECTOR4& v) const
{
	return VECTOR4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
}

VECTOR4 VECTOR4::operator * (float f) const
{
	return VECTOR4(this->x * f, this->y * f, this->z * f, this->w * f);
}

VECTOR4 VECTOR4::operator / (float f) const
{
	float inv = 1.0f / f;
	return VECTOR4(this->x * inv, this->y * inv, this->z * inv, this->w * inv);
}

bool VECTOR4::operator == (const VECTOR4& v)
{
	return (this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w) ? true : false;
}

bool VECTOR4::operator != (const VECTOR4& v)
{
	return (this->x != v.x || this->y != v.y || this->z != v.z || this->w != v.w) ? true : false;
}

bool VECTOR4::operator != (std::nullptr_t t)const
{
	return (this != t) ? true : false;
}



// 内積(2次元)
float VecDot(const VECTOR2 &r1, const VECTOR2 &r2)
{
	return r1.x * r2.x + r1.y * r2.y;
}

// 内積(3次元)
float VecDot(const VECTOR3 &r1, const VECTOR3 &r2)
{
	return r1.x * r2.x + r1.y * r2.y + r1.z * r2.z;
}

// 外積(2次元)
float VecCross(const VECTOR2& r1, const VECTOR2& r2)
{
	return r1.x * r2.y - r1.y * r2.x;
}

// 外積(3次元)
VECTOR3 VecCross(const VECTOR3& r1, const VECTOR3& r2)
{
	return VECTOR3(r1.y * r2.z - r1.z * r2.y, r1.z * r2.x - r1.x * r2.z, r1.x * r2.y - r1.y * r2.x);
}

// 長さの2乗(2次元)
float VecLengthSq(const VECTOR2& v)
{
	return v.x * v.x + v.y * v.y;
}

// 長さ(2次元)
float VecLength(const VECTOR2& v)
{
	return sqrtf(VecLengthSq(v));
}

// 長さの2乗(3次元)
float VecLengthSq(const VECTOR3& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

// 長さ(3次元)
float VecLength(const VECTOR3& v)
{
	return sqrtf(VecLengthSq(v));
}

// 単位ベクトル(2次元)
VECTOR2 VecNorm(const VECTOR2& v)
{
	VECTOR2 temp = v;
	const float len = VecLength(v);
	if (len > 0.0f)
	{
		float inv = 1.0f / len;
		temp.x *= inv;
		temp.y *= inv;
	}
	return temp;
}

// 単位ベクトル(3次元)
VECTOR3 VecNorm(const VECTOR3& v)
{
	VECTOR3 temp = v;
	const float len = VecLength(v);
	if (len > 0.0f)
	{
		float inv = 1.0f / len;
		temp.x *= inv;
		temp.y *= inv;
		temp.z *= inv;
	}
	return temp;
}

// 2次元ベクトルから3次元ベクトルへ(z = 0)
VECTOR3 V3(const VECTOR2& v)
{
	return VECTOR3(v.x, v.y, 0);
}

// 3次元ベクトルから2次元ベクトルへ(z切り捨て)
VECTOR2 V2(const VECTOR3& v)
{
	return VECTOR2(v.x, v.y);
}

// ベクトルの長さ制限(2次元)
VECTOR2 VecRestriction(const VECTOR2& v, float restriction)
{
	VECTOR2 vec = v;
	float l = VecLengthSq(vec);
	float restrictionSq = restriction * restriction;
	if (l > restrictionSq)
	{
		float inv = 1 / (l / restrictionSq);
		vec.x *= inv;
		vec.y *= inv;
	}
	return vec;
}

// ベクトルの長さ制限(3次元)
VECTOR3 VecRestriction(const VECTOR3& v, float restriction)
{
	VECTOR3 vec = v;
	float l = VecLengthSq(vec);
	float restrictionSq = restriction * restriction * restriction;
	if (l > restrictionSq)
	{
		float inv = 1 / (l / restrictionSq);
		vec.x *= inv;
		vec.y *= inv;
		vec.z *= inv;
	}
	return vec;
}

// 円周の範囲を指定するとランダムで速度が返る
VECTOR2 VecCircle(int range)
{
	srand((int)time(NULL) + rand());
	float angle = (float)(rand() % 360);
	float vel = (float)(rand() % range) * 0.01f;
	VECTOR3 velocity = { (sin(angle) * vel), (cos(angle) * vel), 0 };

	return VECTOR2(velocity.x, velocity.y);
}
