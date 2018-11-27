//-----------------------------------------------------------------------------
//
//	3D当たり判定管理[Collider3DManager.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _COLLIDER_3D_MANAGRE_H_
#define _COLLIDER_3D_MANAGRE_H_

#include "../../../Define/Define.h"
#include "../../BaseManager.h"
#include "Collider3DBase.h"
#include "Collider3DOBB.h"
#include "Collider3DPlane.h"
#include "Collider3DSegment.h"
#include "Collider3DSphere.h"

#define _OX_EPSILON_	0.000001f	// 誤差

//-----------------------------------------------------------------------------
//	構造体定義
//-----------------------------------------------------------------------------
// 基本の型
struct Float3
{
	float x;
	float y;
	float z;

	Float3(VECTOR3 d)
	{
		this->x = d.x;
		this->y = d.y;
		this->z = d.z;
	}

	Float3(void) {}
	Float3(float x, float y, float z) : x(x), y(y), z(z) {}
	~Float3(void) {}

	Float3 operator + (const Float3 &r) const
	{
		return Float3(x + r.x, y + r.y, z + r.z);
	}
	Float3 operator - (const Float3 &r) const
	{
		return Float3(x - r.x, y - r.y, z - r.z);
	}
	Float3 operator - (void) const
	{
		return Float3(x * -1.0f, y * -1.0f, z * -1.0f);
	}
	Float3 operator * (const Float3 &r) const
	{
		return Float3(x * r.x, y * r.y, z * r.z);
	}
	Float3 operator / (const Float3 &r) const
	{
		return Float3(x / r.x, y / r.y, z / r.z);
	}
	Float3 operator * (float r) const
	{
		return Float3(x * r, y * r, z * r);
	}
	Float3 operator / (float r) const
	{
		float inv = 1.0f / r;
		return Float3(x * inv, y * inv, z * inv);
	}

	bool operator <= (const Float3 &r)
	{
		if (x <= r.x)
		{
			if (y <= r.y)
			{
				if (z <= r.z)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool operator == (const Float3 &r)
	{
		return x == r.x && y == r.y && z == r.z;
	}
	bool operator != (const Float3 &r)
	{
		return x != r.x || y != r.y || z != r.z;
	}

	friend Float3 operator * (float l, const Float3& r)
	{
		return Float3(r.x * l, r.y * l, r.z * l);
	}
	friend Float3 operator / (float l, const Float3& r)
	{
		return Float3(r.x / l, r.y / l, r.z / l);
	}

	Float3& abs(void)
	{
		if (x < 0) { x *= -1; }
		if (y < 0) { y *= -1; }
		if (z < 0) { z *= -1; }
		return *this;
	}
	Float3& cast(void)
	{
		int cast = (int)(x * 100);
		x = cast * 0.01f;
		cast = (int)(y * 100);
		y = cast * 0.01f;
		cast = (int)(z * 100);
		z = cast * 0.01f;
		return *this;
	}
	float  dot(const Float3 &r) const
	{
		return x * r.x + y * r.y + z * r.z;
	}
	Float3 cross(const Float3 &r) const
	{
		return Float3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
	}
	float  length(void) const
	{
		return sqrtf(lengthSq());
	}
	float  lengthSq(void) const
	{
		return x * x + y * y + z * z;
	}
	void   norm(void)
	{
		const float len = length();
		if (len > 0.0f) {
			x /= len;
			y /= len;
			z /= len;
		}
	}
	Float3 GetNorm(void) const
	{
		const float len = length();
		if (len > 0.0f) {
			return Float3(x / len, y / len, z / len);
		}
		return Float3(0.0f, 0.0f, 0.0f);
	}

	float Get(int i)
	{
		if (i == 0)
		{
			return this->x;
		}
		else if (i == 1)
		{
			return this->y;
		}
		else if (i == 2)
		{
			return this->z;
		}
		return 0;
	}
};

// 点
typedef Float3 Point;

// 3次元ベクトル
struct Vector3 : public Float3
{
	Vector3(void) {}
	Vector3(float x, float y, float z) : Float3(x, y, z) {}
	Vector3(const Float3 &r) : Float3(r) {}
	~Vector3(void) {}

	Vector3& operator =(const Float3 &r)
	{
		x = r.x;
		y = r.y;
		z = r.z;
		return *this;
	}

	// 垂直関係かどうか
	bool isVertical(const Float3 &r) const
	{
		float d = dot(r);
		return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// 誤差範囲内なら垂直と判定
	}

	// 平行関係にあるか
	bool isParallel(const Vector3 &r) const
	{
		float d = cross(r).lengthSq();
		return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// 誤差範囲内なら平行と判定
	}

	// 鋭角関係かどうか
	bool isSharpAngle(const Vector3 &r) const
	{
		return (dot(r) >= 0.0f);
	}
};

// 直線
struct Line
{
	Point   p;
	Vector3 v;
	Line(void) : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}
	Line(const Point &p, const Vector3 &v) : p(p), v(v) {}
	~Line(void) {}

	// 点上の座標を取得
	Float3 GetPoint(float t) const		// tはベクトルに掛ける係数
	{
		return p + t * v;
	}
};

// 線分
struct Segment : public Line
{
	Segment(void) {}
	Segment(const Point &p, const Vector3 &v) : Line(p, v) {}
	Segment(const Point &p1, const Point &p2) : Line(p1, p2 - p1) {}

	// 終点を取得
	Float3 GetEndPoint(void) const
	{
		return p + v;
	}
};

// 球
struct Sphere
{
	Point p;
	float r;	// 半径
	Sphere() : p(0.0f, 0.0f, 0.0f), r(0.5f) {}
	Sphere(const Point &p, float r) : p(p), r(r) {}
	~Sphere() {}
};

// カプセル
struct Capsule
{
	Segment s;
	float r;	// 半径
	Capsule() : r(0.5f) {}
	Capsule(const Segment &s, float r) : s(s), r(r) {}
	Capsule(const Point &p1, const Point &p2, float r) : s(p1, p2), r(r) {}
	~Capsule() {}
};

// AABB
struct AABB {
	Point p;	// 中心点
	Float3 lengthHalf;	// 各軸の辺の長さの半分
	AABB(void) {}
	AABB(const Point &p, const Float3 &hl) : p(p), lengthHalf(hl) {}

	// 辺の長さを取得
	float LengthX(void) const { return lengthHalf.x * 2.0f; };
	float LengthY(void) const { return lengthHalf.y * 2.0f; };
	float LengthZ(void) const { return lengthHalf.z * 2.0f; };
	float Length(int i)
	{
		return *((&lengthHalf.x) + i) * 2.0f;
	}

	float GetMin(int i) const
	{
		if (i == 0)
		{
			return p.x - lengthHalf.x;
		}
		else if (i == 1)
		{
			return p.y - lengthHalf.y;
		}
		else if (i == 2)
		{
			return p.z - lengthHalf.z;
		}
		return 0;
	}
	float GetMax(int i) const
	{
		if (i == 0)
		{
			return p.x + lengthHalf.x;
		}
		else if (i == 1)
		{
			return p.y + lengthHalf.y;
		}
		else if (i == 2)
		{
			return p.z + lengthHalf.z;
		}
		return 0;
	}
};

class Collider3DBase;
class Systems;
//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class Collision3DManager : public BaseManager<Collider3DBase>
{
	friend class Systems;
public:
	~Collision3DManager(void);
	HRESULT Init(void) override { return S_OK; }
	void    Update(void) override;

	friend Collider3DBase;

private:
	Collision3DManager(Systems* systems) : BaseManager(systems) {}

	// 点と線の当たり判定
	bool HitPointSegment(Point p, Segment s);
	// 線と線の当たり判定
	bool HitSegments(Segment s1, Segment s2);
	// 線と平面の当たり判定
	bool HitSegmentPlane(Segment s1, Point p2, Float3 vec2);
	// 線と板ポリゴンの当たり判定
	bool HitSegmentBoard(Segment s1, Float3 n2, std::vector<Point> p2);
	// 筒と筒の当たり判定
	bool HitTubes(Segment s1, float r1, Segment s2, float r2);
	// 球と球の当たり判定
	bool HitSpheres(const Collider3DBase* s1, const Collider3DBase* s2);
	// OBB同士の当たり判定
	bool HitOBBs(const Collider3D::OBB& obb1, const Collider3D::OBB& obb2);
	// OBBと無限平面の当たり判定
	bool HitOBBPlane(const Collider3D::OBB& obb, const Collider3D::Plane& plane, VECTOR3& length);

	float LenSqAABBToPoint(AABB &box, Point &p);
	float LenAABBToPoint(AABB &box, Point &p);

//	float LenSqOBBToPoint(OBB &obb, Point &p);
//	float LenOBBToPoint(OBB &obb, Point &p);
	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(VECTOR3& Sep, VECTOR3& e1, VECTOR3& e2, VECTOR3* e3 = 0);
	// 分離軸A
	bool SeparationA(VECTOR3& Ae, VECTOR3& NAe, VECTOR3& Be1, VECTOR3& Be2, VECTOR3& Be3, VECTOR3& interval);
	// 分離軸B
	bool SeparationB(VECTOR3& Be, VECTOR3& NBe, VECTOR3& Ae1, VECTOR3& Ae2, VECTOR3& Ae3, VECTOR3& interval);
	// 分離軸C
	bool SeparationC(VECTOR3& NAe, VECTOR3& NBe, VECTOR3& Ae1, VECTOR3& Ae2, VECTOR3& Be1, VECTOR3& Be2, VECTOR3& interval);

	void Back(const Collider3D::OBB& obb1, const Collider3D::OBB& obb2, VECTOR3& length);
};

#endif // _COLLISION_3D_MANAGRE_H_

