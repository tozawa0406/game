//-----------------------------------------------------------------------------
//
//	�s��[Matrix.h]
//	Auther : ���V�đ�
//																	2018/08/17
//-----------------------------------------------------------------------------
#ifndef _MATRIX_H_
#define _MATRIX_H_

#pragma warning (disable : 4201)	// �s���struct�̖��O�Ȃ�����W���Ōx�����o��
#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"

//-----------------------------------------------------------------------------
//	�\���̒�`
//-----------------------------------------------------------------------------
struct MATRIX;
struct Transform
{
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;

	VECTOR3 globalPosition;

	const Transform* parent;
	const MATRIX*	 parentMtx;

	Transform(void) : position(0), rotation(0), scale(1, 1, 1), globalPosition(0), parent(0), parentMtx(nullptr){}
	Transform(VECTOR3 position, VECTOR3 rotation, VECTOR3 scale) : position(position), rotation(rotation), scale(scale), globalPosition(position), parent(nullptr), parentMtx(nullptr) {}
};

struct MATRIX
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

	MATRIX(void);								// �R���X�g���N�^

	MATRIX& Identity(void);						// �P�ʍs��
	MATRIX& Translation(const VECTOR3& t);		// �ړ�(3����)
	MATRIX& Translation(const VECTOR2& t);		// �ړ�(2����)
	MATRIX& Rotation(const VECTOR3& r);			// ��](YawPitchRoll)
	MATRIX& Rotation(const VECTOR3& r, float angle);// ��](�C�ӎ�)
	MATRIX& Scaling(const VECTOR3& s);			// �g�k
	MATRIX& Create(const Transform* t);			// Transform�����ɍs��ϊ�
	MATRIX& Transpose(const MATRIX& mtx);		// �]�u�s��
	MATRIX& Billboard(const MATRIX& mtx);		// �r���{�[�h

	MATRIX operator + (const MATRIX& mtx);
	MATRIX operator * (const float f);
	MATRIX operator * (const MATRIX& mtx);
	MATRIX& operator *= (const MATRIX& mtx);
	bool operator == (float n) const;
	bool operator != (float n) const;

	operator float*(void) { return &_11; }
};

//-----------------------------------------------------------------------------
//	�֐���`
//-----------------------------------------------------------------------------
VECTOR3 VecTransformNormal(const VECTOR3& v, const MATRIX& m);		// �s������ɍ��W�ϊ�

#endif //_MATRIX_H_
