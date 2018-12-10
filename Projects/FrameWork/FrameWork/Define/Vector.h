//-----------------------------------------------------------------------------
//
//	�x�N�g��[Vector.h]
//	Auther : ���V�đ�
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
//	�\���̒�`
//-----------------------------------------------------------------------------
struct VECTOR3;
struct VECTOR2
{
public:
	float x, y;

	// �R���X�g���N�^
	VECTOR2(void);
	VECTOR2(float f);
	VECTOR2(float x, float y);

	// ���Z�q�̃I�[�o�[���[�h
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

	// �R���X�g���N�^
	VECTOR3(void);
	VECTOR3(float f);
	VECTOR3(const float x, const float y, const float z);

	// ���Z�q�̃I�[�o�[���[�h
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

	// �R���X�g���N�^
	VECTOR4(void);
	VECTOR4(float f);
	VECTOR4(float x, float y, float z, float w);

	// ���Z�q�̃I�[�o�[���[�h
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

	// �R���X�g���N�^
	VECINT2(void) {}
	VECINT2(int i) : x(i), y(i) {}
	VECINT2(int x, int y) : x(x), y(y) {}
};


//-----------------------------------------------------------------------------
//	�֐���`
//-----------------------------------------------------------------------------
float VecDot(const VECTOR2 &r1, const VECTOR2 &r2);			// ����(2����)
float VecDot(const VECTOR3 &r1, const VECTOR3 &r2);			// ����(3����)
float   VecCross(const VECTOR2& r1, const VECTOR2& r2);		// �O��(2����)
VECTOR3 VecCross(const VECTOR3& r1, const VECTOR3& r2);		// �O��(3����)

float VecLengthSq(const VECTOR2& v);		// ������2��(2����)
float VecLength(const VECTOR2& v);			// ����(2����)
float VecLengthSq(const VECTOR3& v);		// ������2��(3����)
float VecLength(const VECTOR3& v);			// ����(3����)

VECTOR2 VecNorm(const VECTOR2& v);			// �P�ʃx�N�g��(2����)
VECTOR3 VecNorm(const VECTOR3& v);			// �P�ʃx�N�g��(3����)

VECTOR3 V3(const VECTOR2& v);			// 2�����x�N�g������3�����x�N�g����(z = 0)
VECTOR2 V2(const VECTOR3& v);			// 3�����x�N�g������2�����x�N�g����(z�؂�̂�)

VECTOR2 VecRestriction(const VECTOR2& v, float restriction);		// �x�N�g���̒�������(2����)
VECTOR3 VecRestriction(const VECTOR3& v, float restriction);		// �x�N�g���̒�������(3����)

VECTOR2 VecCircle(int range);			// �~���͈̔͂��w�肷��ƃ����_���ő��x���Ԃ�

#endif //_VECTOR_H_
