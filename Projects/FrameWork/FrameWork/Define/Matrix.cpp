//-----------------------------------------------------------------------------
//
//	行列[Matrix.cpp]
//	Auther : 戸澤翔太
//																	2018/08/17
//-----------------------------------------------------------------------------
#include "Matrix.h"

// コンストラクタ
MATRIX::MATRIX(void)
{
	_11 = _12 = _13 = _14 =
		_21 = _22 = _23 = _24 =
		_31 = _32 = _33 = _34 =
		_41 = _42 = _43 = _44 = 0.0f;
}

// 演算子のオーバーロード
MATRIX MATRIX::operator + (const MATRIX& mtx)
{
	MATRIX t;
	t._11 = _11 + mtx._11;
	t._12 = _12 + mtx._12;
	t._13 = _13 + mtx._13;
	t._14 = _14 + mtx._14;
	t._21 = _21 + mtx._21;
	t._22 = _22 + mtx._22;
	t._23 = _23 + mtx._23;
	t._24 = _24 + mtx._24;
	t._31 = _31 + mtx._31;
	t._32 = _32 + mtx._32;
	t._33 = _33 + mtx._33;
	t._34 = _34 + mtx._34;
	t._41 = _41 + mtx._41;
	t._42 = _42 + mtx._42;
	t._43 = _43 + mtx._43;
	t._44 = _44 + mtx._44;
	return t;
}

MATRIX MATRIX::operator * (const float f)
{
	MATRIX t;
	t._11 = _11 * f;
	t._12 = _12 * f;
	t._13 = _13 * f;
	t._14 = _14 * f;
	t._21 = _21 * f;
	t._22 = _22 * f;
	t._23 = _23 * f;
	t._24 = _24 * f;
	t._31 = _31 * f;
	t._32 = _32 * f;
	t._33 = _33 * f;
	t._34 = _34 * f;
	t._41 = _41 * f;
	t._42 = _42 * f;
	t._43 = _43 * f;
	t._44 = _44 * f;
	return t;
}

MATRIX MATRIX::operator * (const MATRIX& mtx)
{
	MATRIX r;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			r.m[i][j] = m[i][0] * mtx.m[0][j] + m[i][1] * mtx.m[1][j] + m[i][2] * mtx.m[2][j] + m[i][3] * mtx.m[3][j];
		}
	}

	return r;
}

MATRIX& MATRIX::operator*=(const MATRIX& mtx)
{
	MATRIX r;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			r.m[i][j] = m[i][0] * mtx.m[0][j] + m[i][1] * mtx.m[1][j] + m[i][2] * mtx.m[2][j] + m[i][3] * mtx.m[3][j];
		}
	}
	*this = r;

	return *this;
}

// 単位行列
MATRIX& MATRIX::Identity(void)
{
	_12 = _13 = _14 =
	_21 = _23 = _24 =
	_31 = _32 = _34 =
	_41 = _42 = _43 = 0.0f;

	_11 = _22 = _33 = _44 = 1.0f;
	return *this;
}

// Tarnsformを元に行列変換
MATRIX& MATRIX::Create(const Transform* t)
{
	Scaling(t->scale);
	Rotation(t->rotation);
	Translation(t->position);

	if (t->parentMtx)
	{
		MATRIX temp = *t->parentMtx;
		*this *= temp;
	}
	if (t->parent)
	{
		Create(t->parent);
	}
	const_cast<Transform*>(t)->globalPosition = VECTOR3(_41, _42, _43);

	return *this;
}

// 移動(3次元)
MATRIX& MATRIX::Translation(const VECTOR3& t)
{
	MATRIX temp;
	temp.Identity();
	temp._41 = t.x;
	temp._42 = t.y;
	temp._43 = t.z;

	*this *= temp;

	return *this;
}

// 移動(2次元)
MATRIX& MATRIX::Translation(const VECTOR2& t)
{
	MATRIX temp;
	temp.Identity();
	temp._41 = t.x;
	temp._42 = t.y;

	*this *= temp;

	return *this;
}

// 回転(YawPitchRoll)
MATRIX& MATRIX::Rotation(const VECTOR3& r)
{
	MATRIX temp;
	temp.Identity();
	temp._11 = cosf(r.y) * cosf(r.z);
	temp._12 = -cosf(r.y) * sinf(r.z);
	temp._13 = sinf(r.y);
	temp._21 = cosf(r.x) * sinf(r.z) + sinf(r.x) * sinf(r.y) * cosf(r.z);
	temp._22 = cosf(r.x) * cosf(r.z) - sinf(r.x) * sinf(r.y) * sinf(r.z);
	temp._23 = -sinf(r.x) * cosf(r.y);
	temp._31 = sinf(r.x) * sinf(r.z) - cosf(r.x) * sinf(r.y) * cosf(r.z);
	temp._32 = sinf(r.x) * cosf(r.z) + cosf(r.x) * sinf(r.y) * sinf(r.z);
	temp._33 = cosf(r.x) * cosf(r.y);
	*this *= temp;

	return *this;
}

// 回転(任意軸)
MATRIX& MATRIX::Rotation(const VECTOR3& r, float angle)
{
	MATRIX temp;
	temp.Identity();

	float cos = cosf(angle);
	float cosInv = 1.0f - cos;
	float sin = sinf(angle);

	temp._11 = r.x * r.x * cosInv + cos;
	temp._12 = r.x * r.y * cosInv - r.z * sin;
	temp._13 = r.x * r.z * cosInv + r.y * sin;
	temp._21 = r.x * r.y * cosInv + r.z * sin;
	temp._22 = r.y * r.y * cosInv + cos;
	temp._23 = r.y * r.z * cosInv - r.x * sin;
	temp._31 = r.x * r.z * cosInv - r.y * sin;
	temp._32 = r.y * r.z * cosInv + r.x * sin;
	temp._33 = r.z * r.z * cosInv + cos;

	*this *= temp;

	return *this;
}


// 拡縮
MATRIX& MATRIX::Scaling(const VECTOR3& s)
{
	MATRIX temp;
	temp.Identity();
	temp._11 = s.x;
	temp._22 = s.y;
	temp._33 = s.z;

	*this *= temp;

	return *this;
}

// 転置行列
MATRIX& MATRIX::Transpose(const MATRIX& mtx)
{
	*this = mtx;

	_12 = mtx._21;
	_13 = mtx._31;
	_14 = mtx._41;

	_21 = mtx._12;
	_23 = mtx._32;
	_24 = mtx._42;

	_31 = mtx._13;
	_32 = mtx._23;
	_34 = mtx._43;

	_41 = mtx._14;
	_42 = mtx._24;
	_43 = mtx._34;

	return *this;
}

// ビルボード
MATRIX& MATRIX::Billboard(const MATRIX& mtx)
{
	Transpose(mtx);

	_14 = _24 = _34 = 0;

	return *this;
}

// ==比較
bool MATRIX::operator == (float n) const
{
	if (((_11 == n) && (_12 == n)) && ((_13 == n) && (_14 == n)))
	{
		if (((_21 == n) && (_22 == n)) && ((_23 == n) && (_24 == n)))
		{
			if (((_31 == n) && (_32 == n)) && ((_33 == n) && (_34 == n)))
			{
				if (((_41 == n) && (_42 == n)) && ((_43 == n) && (_44 == n)))
				{
					return true;
				}
			}
		}
	}
	return false;
}

// !=比較
bool MATRIX::operator != (float n) const
{
	return !operator==(n);
}



// 行列を元に座標変換
VECTOR3 VecTransform(const VECTOR3& v, const MATRIX& m)
{
	float r[4];
	for (int i = 0; i < 4; ++i)
	{
		r[i] = v.x * m.m[0][i] + v.y * m.m[1][i] + v.z * m.m[2][i] + m.m[3][i];
	}

	return VECTOR3(r[0], r[1], r[2]);
}


// 行列を元に座標変換
VECTOR3 VecTransformCoord(const VECTOR3& v, const MATRIX& m)
{
	float r[4];
	for (int i = 0; i < 4; ++i)
	{
		r[i] = v.x * m.m[0][i] + v.y * m.m[1][i] + v.z * m.m[2][i] + m.m[3][i];
	}

	float inv = 1 / r[3];
	return VECTOR3(r[0] * inv, r[1] * inv, r[2] * inv);
}
