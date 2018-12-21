/*
 * @file	Matrix.h
 * @brief	行列
 * @author	戸澤翔太
 * @date	2018/08/17
 */
#ifndef _MATRIX_H_
#define _MATRIX_H_

#pragma warning (disable : 4201)	// 行列のstructの名前なしが非標準で警告が出る
#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"

struct MATRIX;
struct Transform
{
	VECTOR3 position;		//! 位置
	VECTOR3 rotation;		//! 回転角度
	VECTOR3 scale;			//! 拡縮

	VECTOR3 globalPosition;		//! グローバル空間での座標

	const Transform* parent;		//! 親
	const MATRIX*	 parentMtx;		//! 親の行列

	/* @brief	コンストラクタ		*/
	Transform(void);

	/* @brief	コンストラクタ
	 * @param	(position)	位置
	 * @param	(rotation)	回転角度
	 * @param	(scale)		拡縮	*/
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

	/* @brief	コンストラクタ		*/
	MATRIX(void);

	/* @brief	単位行列			*/
	MATRIX& Identity(void);

	/* @brief	3次元移動
	 * @param	(t)		移動ベクトル		*/
	MATRIX& Translation(const VECTOR3& t);
	
	/* @brief	2次元移動
	 * @param	(t)		移動ベクトル		*/
	MATRIX& Translation(const VECTOR2& t);
	
	/* @brief	3次元回転
	 * @param	(r)		YawPitchRoll		*/
	MATRIX& Rotation(const VECTOR3& r);
	
	/* @brief	3次元回転
	 * @param	(r)		回転軸
	 * @param	(angle)	回転角度			*/
	MATRIX& Rotation(const VECTOR3& r, float angle);

	/* @brief	3次元拡縮
	 * @param	(s)		拡縮				*/
	MATRIX& Scaling(const VECTOR3& s);

	/* @brief	姿勢行列の生成
	 * @param	(t)		姿勢				*/
	MATRIX& Create(const Transform* t);

	/* @brief	転置行列
	 * @param	(mtx)	転置する行列		*/
	MATRIX& Transpose(const MATRIX& mtx);

	/* @brief	ビルボード行列
	 * @param	(mtx)	ビルボード処理する行列		*/
	MATRIX& Billboard(const MATRIX& mtx);

	MATRIX operator + (const MATRIX& mtx);
	MATRIX operator * (const float f);
	MATRIX operator * (const MATRIX& mtx);
	MATRIX& operator *= (const MATRIX& mtx);
	bool operator == (float n) const;
	bool operator != (float n) const;

	operator float*(void) { return &_11; }
};

/* @brief	行列を元に座標変換
 * @param	(v)		座標変換するベクトル
 * @param	(m)		元にする行列			*/
VECTOR3 VecTransform(const VECTOR3& v, const MATRIX& m);

/* @brief	行列を元に座標変換
 * @param	(v)		座標変換するベクトル
 * @param	(m)		元にする行列			*/
VECTOR3 VecTransformCoord(const VECTOR3& v, const MATRIX& m);

/* @brief	View変換行列の生成
 * @param	(position)	位置
 * @param	(at)		注視点
 * @param	(up)		上ベクトル
 * @return	View変換行列					*/
MATRIX  CreateViewMatrix(const VECTOR3& position, const VECTOR3& at, const VECTOR3& up);

/* @brief	Projection変換行列
 * @param	(fov)		0～360の角度
 * @param	(aspect)	アスペクト比
 * @param	(cnear)		near
 * @param	(cfar)		far
 * @return	Projection変換行列				*/
MATRIX  CreateProjectionMatrix(int fov, float aspect, float cnear, float cfar);

#endif //_MATRIX_H_
