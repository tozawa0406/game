//-----------------------------------------------------------------------------
//
//	行列[Matrix.h]
//	Auther : 戸澤翔太
//																	2018/08/17
//-----------------------------------------------------------------------------
#ifndef _MATRIX_H_
#define _MATRIX_H_

#pragma warning (disable : 4201)	// 行列のstructの名前なしが非標準で警告が出る
#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"

//-----------------------------------------------------------------------------
//	構造体定義
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

	MATRIX(void);								// コンストラクタ

	MATRIX& Identity(void);						// 単位行列
	MATRIX& Translation(const VECTOR3& t);		// 移動(3次元)
	MATRIX& Translation(const VECTOR2& t);		// 移動(2次元)
	MATRIX& Rotation(const VECTOR3& r);			// 回転(YawPitchRoll)
	MATRIX& Rotation(const VECTOR3& r, float angle);// 回転(任意軸)
	MATRIX& Scaling(const VECTOR3& s);			// 拡縮
	MATRIX& Create(const Transform* t);			// Transformを元に行列変換
	MATRIX& Transpose(const MATRIX& mtx);		// 転置行列
	MATRIX& Billboard(const MATRIX& mtx);		// ビルボード

	MATRIX operator + (const MATRIX& mtx);
	MATRIX operator * (const float f);
	MATRIX operator * (const MATRIX& mtx);
	MATRIX& operator *= (const MATRIX& mtx);
	bool operator == (float n) const;
	bool operator != (float n) const;

	operator float*(void) { return &_11; }
};

//-----------------------------------------------------------------------------
//	関数定義
//-----------------------------------------------------------------------------
VECTOR3 VecTransformNormal(const VECTOR3& v, const MATRIX& m);		// 行列を元に座標変換

#endif //_MATRIX_H_
