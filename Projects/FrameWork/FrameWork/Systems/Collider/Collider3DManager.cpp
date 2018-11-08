//-----------------------------------------------------------------------------
//
//	3D当たり判定管理[Collider3DManager.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Collider3D.h"
#include "../GameSystems.h"
#include "../../Object/Object.h"
#include "../../Object/ObjectManager.h"
#include "Collider3D.h"

Collision3DManager::~Collision3DManager(void)
{
	for (UINT i = 0; i < obj_.size();)
	{
		this->RemovePtr(obj_[i]);
	}
}

void Collision3DManager::Update(void)
{
	for (auto obj : obj_)
	{
		obj->list_.clear();
		obj->colliderList_.clear();

		obj->Update();
	}

	for (auto col1 : obj_)
	{
		if (!col1->enable_) { continue; }
		for (auto col2 : obj_)
		{
			if (!col2->enable_) { continue; }
			if (col1->object_->GetTag() == Object::Tag::STATIC && col2->object_->GetTag() == Object::Tag::STATIC) { continue; }

			if (col1 != col2)
			{
				bool isList = false;
				for (auto obj : col2->list_)
				{
					if (obj == col1->object_) 
					{
						col1->colliderList_.emplace_back(col2);
						col1->list_.push_back(col2->object_);
						isList = true; 
					}
				}
				if (isList) { continue; }

				if (col1->type_ == Collider3DBase::Type::SPHERE)
				{
					if (col2->type_ == Collider3DBase::Type::SPHERE)
					{
						if(this->HitSpheres(col1, col2))
						{
							col1->list_.push_back(col2->object_);
							col1->colliderList_.emplace_back(col2);
						}
					}
				}
				else if (col1->type_ == Collider3DBase::Type::OBB)
				{
					if (col2->type_ == Collider3DBase::Type::OBB)
					{
						if(this->HitOBBs((*(Collider3D::OBB*)col1), (*(Collider3D::OBB*)col2)))
						{
							col1->list_.push_back(col2->object_);
							col1->colliderList_.emplace_back(col2);
						}
					}
				}
			}
		}
	}

}

// 点と線の当たり判定
bool Collision3DManager::HitPointSegment(Point p, Segment s)
{
	// 線分
	Vector3 segment = Half(Vector3(s.GetEndPoint().x - s.p.x, s.GetEndPoint().y - s.p.y, s.GetEndPoint().z - s.p.z));
	// 点から線分の始点へ向かうベクトル
	Vector3 vec2    =  p - (s.p + segment);

	//	| v1×v | が0、かつ | v1 | ≦ | v | ならば衝突を起こしている
	if (segment.isParallel(vec2))
	{
		if (vec2.lengthSq() <= segment.lengthSq())
		{
 			return true;
		}
	}
	return false;
}

// 線と線の当たり判定
bool Collision3DManager::HitSegments(Segment s1, Segment s2)
{
	// ベクトル1
	Float3 end = s1.GetEndPoint();
	Vector3 vec1 = Float3(end.x - s1.p.x, end.y - s1.p.y, end.z - s1.p.z);
	// ベクトル2
	end = s2.GetEndPoint();
	Vector3 vec2 = Float3(end.x - s2.p.x, end.y - s2.p.y, end.z - s2.p.z);
	// ベクトル1とベクトル2の中点の距離
	Vector3 vec3 = (s1.p + Half(vec1)) - (s2.p + Half(vec2));

	// ベクトル1とベクトル2の外積
	Vector3 n1 = vec1.cross(vec2);
	// ベクトル1と差との外積
	Vector3 n2 = vec1.cross(vec3);

	// 2辺が平行であるか
	float sq = n2.cast().lengthSq();
	// 平行であるならば
	if (sq < 0.01f)
	{
		// ベクトル1の半分とベクトル2の半分を足す
		Vector3 temp = Half(vec1.cast().abs()) + Half(vec2.cast().abs());
		// 距離
		sq = vec3.cast().lengthSq();

		// 円の当たり判定
		if (sq < temp.lengthSq()) { return true; }
	}
	// 外積が平行であるとき
	if (n1.isParallel(n2))
	{
		end = s2.GetEndPoint();
		// ベクトル1の始点がベクトル2の始点か終点より大きいとき
		if (s2.p <= s1.p || end <= s1.p)
		{
			end = s1.GetEndPoint();
			// ベクトル1の終点がベクトル2の始点か終点より小さいとき
			if (end <= s2.p || end <= s2.GetEndPoint())
			{
				return true;
			}
		}
		// ベクトル1の始点がベクトル2の始点か終点より小さいとき
		else if (s1.p <= s2.p || s1.p <= end)
		{
			end = s1.GetEndPoint();
			// ベクトル1の終点がベクトル2の始点か終点より大きいとき
			if (s2.p <= end || s2.GetEndPoint() <= end)
			{
				return true;
			}
		}
	}
	return false;
}

// 線と平面の当たり判定
bool Collision3DManager::HitSegmentPlane(Segment s1, Point p2, Float3 n2)
{
	// 線の始点から平面の一点までのベクトル
	Vector3 vec1 = s1.p - p2;
	// 線の終点から平面の一点までのベクトル
	Vector3 vec2 = s1.GetEndPoint() - p2;

	// どちらかのベクトルが内積で0以下だった場合に衝突
	if (vec1.dot(n2) * vec2.dot(n2) <= 0)
	{
		return true;
	}
	return false;
}

// 線と板ポリゴンの当たり判定
bool Collision3DManager::HitSegmentBoard(Segment s1, Float3 n2, std::vector<Point> p2)
{
	if (p2.size() == 0) { return false; }
	// 線の始点から平面の一点までのベクトル
	Vector3 vec1 = s1.p - p2[0];
	// 線の終点から平面の一点までのベクトル
	Vector3 vec2 = s1.GetEndPoint() - p2[0];

	// どちらかのベクトルが内積で0以下だった場合に平面を貫通
	if (vec1.dot(n2) * vec2.dot(n2) <= 0)
	{
		// 点から平面までの距離
		// 始点
		Vector3 v1 = s1.p - p2[0];
		float d1 = fabs(n2.GetNorm().dot(v1));
		// 終点
		Vector3 v2 = s1.GetEndPoint() - p2[0];
		float d2 = fabs(n2.GetNorm().dot(v2));

		// 内分比
		float a = d1 / (d1 + d2);
		// 最初の頂点から貫通点までの距離
		Vector3 vec3 = (1 - a) * vec1.cast() + a * vec2.cast();

		// ポリゴン平面までの距離から内分比を算出して貫通点の座標を確定
		Point p3 = p2[0] + vec3.cast();
			
		// ポリゴン内部に貫通点が含まれるかをチェック
		bool hit = false;
		// トライアングルストリップのせいで入るループ
		for (UINT j = 0; j < p2.size() - 2; j++)
		{
			// 初期化
			Vector3 cross  = { 1, 1, 1 };
			bool check = false;
			// トライアングルストリップだからな！！！！！！！！！！！！！！！！！！！！
			for (UINT i = j; i < 3 + j; i++)
			{
				Vector3 vector1;
				// 次の頂点へのベクトル
				if (i < j + 2) { vector1 = p2[i + 1] - p2[i]; }
				// 最後は最初の頂点へのベクトル
				else           { vector1 = p2[j]     - p2[i]; }
				// 貫通点から現在の頂点へのベクトル
				Vector3 vector2 = p2[i] - p3;

				// 上記二つのベクトルの外積
				Vector3 c = vector1.cross(vector2).GetNorm();
				// 外積が他と違えば貫通点はポリゴンの外
				if (cross != c)
				{
					// 外積が初期化の状態でなければ当たってない
					if (cross != Vector3(1, 1, 1))
					{
						check = true;
					}
				}
				cross = c;
			}					
			if (!check) { hit = true; }
		}
		return hit;
	}
	return false;
}

// 筒と筒の当たり判定
bool Collision3DManager::HitTubes(Segment s1, float r1, Segment s2, float r2)
{
	Vector3 vec = s2.p - s1.p;
	Vector3 normal = s1.v.cross(s2.v);

	float d = fabs(normal.GetNorm().dot(vec));
	if (d <= r1 + r2)
	{
		return true;
	}
	return false;
}

// 球と球の当たり判定
bool Collision3DManager::HitSpheres(const Collider3DBase* s1, const Collider3DBase* s2)
{
	Point diff = Float3(s2->transform_.position.x - s1->transform_.position.x, s2->transform_.position.y - s1->transform_.position.y, s2->transform_.position.z - s1->transform_.position.z);
	float r = Half(s1->size_.x) + Half(s2->size_.x);

	return (diff.lengthSq() <= r * r) ? true : false;
}

// OBB同士の当たり判定
bool Collision3DManager::HitOBBs(const Collider3D::OBB& obb1, const Collider3D::OBB& obb2)
{
	// 各方向ベクトルの確保
	VECTOR3 NAe[3], NBe[3], Ae[3], Be[3];
	for (int i = 0; i < 3; ++i)
	{
		NAe[i] = obb1.GetDirect(i);
		Ae[i]  = NAe[i] * obb1.GetLen(i);
		NBe[i] = obb2.GetDirect(i);
		Be[i]  = NBe[i] * obb2.GetLen(i);
	}
	VECTOR3 interval = obb1.transform_.position - obb2.transform_.position;

	for (int i = 0; i < 3; ++i)
	{
		// 分離軸 : Ae
		if (!SeparationA(Ae[i], NAe[i], Be[0], Be[1], Be[2], interval)) { return false; }
		// 分離軸 : Be
		if (!SeparationB(Be[i], NBe[i], Ae[0], Ae[1], Ae[2], interval)) { return false; }
	}

	// 分離軸 : C11
	if (!SeparationC(NAe[0], NBe[0], Ae[1], Ae[2], Be[1], Be[2], interval)) { return false; }
	// 分離軸 : C12
	if (!SeparationC(NAe[0], NBe[1], Ae[1], Ae[2], Be[0], Be[2], interval)) { return false; }
	// 分離軸 : C13
	if (!SeparationC(NAe[0], NBe[2], Ae[1], Ae[2], Be[0], Be[1], interval)) { return false; }

	// 分離軸 : C21
	if (!SeparationC(NAe[1], NBe[0], Ae[0], Ae[2], Be[1], Be[2], interval)) { return false; }
	// 分離軸 : C22
	if (!SeparationC(NAe[1], NBe[1], Ae[0], Ae[2], Be[0], Be[2], interval)) { return false; }
	// 分離軸 : C23
	if (!SeparationC(NAe[1], NBe[2], Ae[0], Ae[2], Be[0], Be[1], interval)) { return false; }

	// 分離軸 : C31
	if (!SeparationC(NAe[2], NBe[0], Ae[0], Ae[1], Be[1], Be[2], interval)) { return false; }
	// 分離軸 : C32
	if (!SeparationC(NAe[2], NBe[1], Ae[0], Ae[1], Be[0], Be[2], interval)) { return false; }
	// 分離軸 : C33
	if (!SeparationC(NAe[2], NBe[2], Ae[0], Ae[1], Be[0], Be[1], interval)) { return false; }

	// 分離平面が存在しないので「衝突している」
	return true;
}

float Collision3DManager::LenSqAABBToPoint(AABB &box, Point &p)
{
	FLOAT SqLen = 0;   // 長さのべき乗の値を格納
	int i;
	for (i = 0; i<3; i++)
	{
		// 各軸で点が最小値以下もしくは最大値以上ならば、差を考慮
		if (p.Get(i) < box.GetMin(i))  // i = 0 = X、1 = Y、2 = Z
			SqLen += (p.Get(i) - box.GetMin(i)) * (p.Get(i) - box.GetMin(i));
		if (p.Get(i) > box.GetMax(i))
			SqLen += (p.Get(i) - box.GetMax(i)) * (p.Get(i) - box.GetMax(i));
	}
	return SqLen;
}

float Collision3DManager::LenAABBToPoint(AABB &box, Point &p)
{
	return sqrt(LenSqAABBToPoint(box, p));
}

float Collision3DManager::LenSqOBBToPoint(OBB &obb, Point &p)
{
	Vector3 vec(0, 0, 0);

	for (int i = 0; i < 3; i++)	// 各軸についてはみ出た部分のベクトルを算出
	{
		FLOAT L = obb.GetLen(i);
		if (L <= 0) { continue; }  // L=0は計算できない
		float s = Vector3(p - obb.GetPos()).dot(obb.GetDirect(i)) / L;

		// sの値から、はみ出した部分があればそのベクトルを加算
		s = fabs(s);
		if (s > 1)
		{
			vec = vec + (1 - s) * L * obb.GetDirect(i);   // はみ出した部分のベクトル算出
		}
	}
	return vec.lengthSq();
}

float Collision3DManager::LenOBBToPoint(OBB &obb, Point &p)
{
	return sqrt(LenSqOBBToPoint(obb, p));
}

// 分離軸に投影された軸成分から投影線分長を算出
float Collision3DManager::LenSegOnSeparateAxis(VECTOR3& Sep, VECTOR3& e1, VECTOR3& e2, VECTOR3* e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	float r1 = fabs(VecDot(Sep, e1));
	float r2 = fabs(VecDot(Sep, e2));
	float r3 = e3 ? (fabs(VecDot(Sep, *e3))) : 0;
	return r1 + r2 + r3;
}

// 分離軸A
bool Collision3DManager::SeparationA(VECTOR3& Ae, VECTOR3& NAe, VECTOR3& Be1, VECTOR3& Be2, VECTOR3& Be3, VECTOR3& interval)
{
	float rA = VecLength(Ae);
	float rB = LenSegOnSeparateAxis(NAe, Be1, Be2, &Be3);
	float L = fabs(VecDot(interval, NAe));
	if (L > rA + rB)
	{
		return false; // 衝突していない
	}
	return true;
}

// 分離軸B
bool Collision3DManager::SeparationB(VECTOR3& Be, VECTOR3& NBe, VECTOR3& Ae1, VECTOR3& Ae2, VECTOR3& Ae3, VECTOR3& interval)
{
	float rA = LenSegOnSeparateAxis(NBe, Ae1, Ae2, &Ae3);
	float rB = VecLength(Be);
	float L = fabs(VecDot(interval, NBe));
	if (L > rA + rB)
	{
		return false;	// 衝突していない
	}
	return true;
}

// 分離軸C
bool Collision3DManager::SeparationC(VECTOR3& NAe, VECTOR3& NBe, VECTOR3& Ae1, VECTOR3& Ae2, VECTOR3& Be1, VECTOR3& Be2, VECTOR3& interval)
{
	VECTOR3 cross = VecCross(NAe, NBe);
	float rA = LenSegOnSeparateAxis(cross, Ae1, Ae2);
	float rB = LenSegOnSeparateAxis(cross, Be1, Be2);
	float L = fabs(VecDot(interval, cross));
	if (L > rA + rB)
	{
		return false;
	}
	return true;
}
