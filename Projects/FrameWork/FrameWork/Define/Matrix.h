/*
 * @file	Matrix.h
 * @brief	�s��
 * @author	���V�đ�
 * @date	2018/08/17
 */
#ifndef _MATRIX_H_
#define _MATRIX_H_

#pragma warning (disable : 4201)	// �s���struct�̖��O�Ȃ�����W���Ōx�����o��
#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"

struct MATRIX;
struct Transform
{
	VECTOR3 position;		//! �ʒu
	VECTOR3 rotation;		//! ��]�p�x
	VECTOR3 scale;			//! �g�k

	VECTOR3 globalPosition;		//! �O���[�o����Ԃł̍��W

	const Transform* parent;		//! �e
	const MATRIX*	 parentMtx;		//! �e�̍s��

	/* @brief	�R���X�g���N�^		*/
	Transform(void);

	/* @brief	�R���X�g���N�^
	 * @param	(position)	�ʒu
	 * @param	(rotation)	��]�p�x
	 * @param	(scale)		�g�k	*/
	Transform(const VECTOR3& position, const VECTOR3& rotation, const VECTOR3& scale);
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

	/* @brief	�R���X�g���N�^		*/
	MATRIX(void);

	/* @brief	�P�ʍs��			*/
	MATRIX& Identity(void);

	/* @brief	3�����ړ�
	 * @param	(t)		�ړ��x�N�g��		*/
	MATRIX& Translation(const VECTOR3& t);
	
	/* @brief	2�����ړ�
	 * @param	(t)		�ړ��x�N�g��		*/
	MATRIX& Translation(const VECTOR2& t);
	
	/* @brief	3������]
	 * @param	(r)		YawPitchRoll		*/
	MATRIX& Rotation(const VECTOR3& r);
	
	/* @brief	3������]
	 * @param	(r)		��]��
	 * @param	(angle)	��]�p�x			*/
	MATRIX& Rotation(const VECTOR3& r, float angle);

	/* @brief	3�����g�k
	 * @param	(s)		�g�k				*/
	MATRIX& Scaling(const VECTOR3& s);

	/* @brief	�p���s��̐���
	 * @param	(t)		�p��				*/
	MATRIX& Create(const Transform* t);

	/* @brief	�]�u�s��
	 * @param	(mtx)	�]�u����s��		*/
	MATRIX& Transpose(const MATRIX& mtx);

	/* @brief	�r���{�[�h�s��
	 * @param	(mtx)	�r���{�[�h��������s��		*/
	MATRIX& Billboard(const MATRIX& mtx);

	MATRIX operator + (const MATRIX& mtx);
	MATRIX operator * (const float f);
	MATRIX operator * (const MATRIX& mtx);
	MATRIX& operator *= (const MATRIX& mtx);
	bool operator == (float n) const;
	bool operator != (float n) const;

	operator float*(void) { return &_11; }
};

/* @brief	�s������ɍ��W�ϊ�
 * @param	(v)		���W�ϊ�����x�N�g��
 * @param	(m)		���ɂ���s��			*/
VECTOR3 VecTransform(const VECTOR3& v, const MATRIX& m);

/* @brief	�s������ɍ��W�ϊ�
 * @param	(v)		���W�ϊ�����x�N�g��
 * @param	(m)		���ɂ���s��			*/
VECTOR3 VecTransformCoord(const VECTOR3& v, const MATRIX& m);

/* @brief	View�ϊ��s��̐���
 * @param	(position)	�ʒu
 * @param	(at)		�����_
 * @param	(up)		��x�N�g��
 * @return	View�ϊ��s��					*/
MATRIX  CreateViewMatrix(const VECTOR3& position, const VECTOR3& at, const VECTOR3& up);

/* @brief	Projection�ϊ��s��
 * @param	(fov)		0�`360�̊p�x
 * @param	(aspect)	�A�X�y�N�g��
 * @param	(cnear)		near
 * @param	(cfar)		far
 * @return	Projection�ϊ��s��				*/
MATRIX  CreateProjectionMatrix(int fov, float aspect, float cnear, float cfar);

#endif //_MATRIX_H_
