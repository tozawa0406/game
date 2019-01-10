/*
 * @file	Vector.h
 * @brief	�x�N�g��
 * @author	���V�đ�
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

	/* @brief	�R���X�g���N�^		*/
	VECTOR2(void);

	/* @brief	�R���X�g���N�^
	 * @param	(f)		�������l	*/
	VECTOR2(float f);
	
	/* @brief	�R���X�g���N�^
	 * @param	(x)		x�̏������l
	 * @param	(y)		y�̏������l	*/
	VECTOR2(float x, float y) :x(x), y(y) {}

	template<class T1, class T2>
	VECTOR2(const T1 x, const T2 y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}

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

	/* @brief	�R���X�g���N�^		*/
	VECTOR3(void);

	/* @brief	�R���X�g���N�^
	 * @param	(f)		�������l	*/
	VECTOR3(float f);

	/* @brief	�R���X�g���N�^
	 * @param	(x)		x�̏������l
	 * @param	(y)		y�̏������l
	 * @param	(z)		z�̏������l	*/
	VECTOR3(const float x, const float y, const float z);

	template<class T1, class T2, class T3>
	VECTOR3(const T1 x, const T2 y, const T3 z) : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z)) {}

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

	/* @brief	�R���X�g���N�^		*/
	VECTOR4(void);

	/* @brief	�R���X�g���N�^
	 * @param	(f)		�������l	*/
	VECTOR4(float f);
	
	/* @brief	�R���X�g���N�^
	 * @param	(x)		x�̏������l
	 * @param	(y)		y�̏������l
	 * @param	(z)		z�̏������l
	 * @param	(w)		w�̏������l	*/
	VECTOR4(float x, float y, float z, float w);

	template<class T1, class T2, class T3, class T4>
	VECTOR4(const T1 x, const T2 y, const T3 z, const T4 w) : x(static_cast<float>(x)), y(static_cast<float>(y))
															, z(static_cast<float>(z)), w(static_cast<float>(w)) {}

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

	/* @brief	�R���X�g���N�^		*/
	VECINT2(void);

	/* @brief	�R���X�g���N�^
	 * @param	(i)		�������l	*/
	VECINT2(int i);

	/* @brief	�R���X�g���N�^
	 * @param	(x)		x�̏������l
	 * @param	(y)		y�̏������l	*/
	VECINT2(int x, int y);
};

/* @brief	2�����̓���
 * @param	(r1)	�x�N�g��1
 * @param	(r2)	�x�N�g��2
 * @return	����					*/
float VecDot(const VECTOR2 &r1, const VECTOR2 &r2);

/* @brief	3�����̓���
 * @param	(r1)	�x�N�g��1
 * @param	(r2)	�x�N�g��2
 * @return	����					*/
float VecDot(const VECTOR3 &r1, const VECTOR3 &r2);

/* @brief	2�����̊O��
 * @param	(r1)	�x�N�g��1
 * @param	(r2)	�x�N�g��2
 * @return	�O��					*/
float   VecCross(const VECTOR2& r1, const VECTOR2& r2);

/* @brief	3�����̊O��
 * @param	(r1)	�x�N�g��1
 * @param	(r2)	�x�N�g��2
 * @return	�O��					*/
VECTOR3 VecCross(const VECTOR3& r1, const VECTOR3& r2);



/* @brief	2�����̒�����2��
 * @param	(v)		�x�N�g��
 * @return	������2��				*/
float VecLengthSq(const VECTOR2& v);

/* @brief	2�����̒���
 * @param	(v)		�x�N�g��
 * @return	����					*/
float VecLength(const VECTOR2& v);

/* @brief	3�����̒�����2��
 * @param	(v)		�x�N�g��
 * @return	������2��				*/
float VecLengthSq(const VECTOR3& v);

/* @brief	3�����̒���
 * @param	(v)		�x�N�g��
 * @return	����					*/
float VecLength(const VECTOR3& v);


/* @brief	2�����̒P�ʃx�N�g��
 * @param	(v)		�x�N�g��
 * @return	�P�ʃx�N�g��			*/
VECTOR2 VecNorm(const VECTOR2& v);

/* @brief	3�����̒P�ʃx�N�g��
 * @param	(v)		�x�N�g��
 * @return	�P�ʃx�N�g��			*/
VECTOR3 VecNorm(const VECTOR3& v);


/* @brief	2��������3������
 * @param	(v)		�x�N�g��
 * @return	3�����x�N�g��(z = 0)		*/
VECTOR3 V3(const VECTOR2& v);

/* @brief	3��������2������
 * @param	(v)		�x�N�g��
 * @return	2�����x�N�g��(z�؂�̂�)	*/
VECTOR2 V2(const VECTOR3& v);

/* @brief	2�����̃x�N�g����������
 * @param	(v)				�x�N�g��
 * @param	(restriction)	�����l
 * @return	�������ꂽ�x�N�g��			*/
VECTOR2 VecRestriction(const VECTOR2& v, float restriction);

/* @brief	3�����̃x�N�g����������
 * @param	(v)				�x�N�g��
 * @param	(restriction)	�����l
 * @return	�������ꂽ�x�N�g��			*/
VECTOR3 VecRestriction(const VECTOR3& v, float restriction);

/* @brief	�~���͈̔͂��w�肷��ƃ����_���ő��x���Ԃ�
 * @param	(range)		�͈�
 * @return	���x						*/
VECTOR2 VecCircle(int range);

#endif //_VECTOR_H_
