//-----------------------------------------------------------------------------
//
//	3D当たり判定管理[Collider3DManager.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "Collider3DManager.h"
#include "../../GameSystems.h"
#include "../../../Object/Object.h"
#include "../../../Object/ObjectManager.h"

Collision3DManager::~Collision3DManager(void)
{
	for (uint i = 0; i < obj_.size();)
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

		obj->back_ = VECTOR3(0, 0, 0);
		obj->Update();
		MATRIX().Identity().Create(&obj->transform_);
	}

	for (auto col1 : obj_)
	{
		// 不使用
		if (!col1->enable_) { continue; }
		for (auto col2 : obj_)
		{
			// 不使用
			if (!col2->enable_) { continue; }
			// 同一
			if (col1 == col2) { continue; }
			// 同一の親
			if (col1->object_ == col2->object_) { continue; }
			// 動かないオブジェクト同士
			if (col1->object_->GetTag() == ObjectTag::STATIC && col2->object_->GetTag() == ObjectTag::STATIC) { continue; }

			// 球
			if (col1->type_ == Collider3DBase::Type::SPHERE)
			{
				
				if (col2->type_ == Collider3DBase::Type::SPHERE)
				{
					if(this->HitSpheres(col1, col2))
					{
						col1->list_.emplace_back(col2->object_);
						col1->colliderList_.emplace_back(col2);
					}
				}
			}
			// OBB
			else if (col1->type_ == Collider3DBase::Type::OBB)
			{
				Collider3D::OBB* obb1 = static_cast<Collider3D::OBB*>(col1);
				// OBB
				if (col2->type_ == Collider3DBase::Type::OBB)
				{
					Collider3D::OBB* obb2 = static_cast<Collider3D::OBB*>(col2);
					if(this->HitOBBs(*obb1, *obb2))
					{
						if (col1->renderer_.color == COLOR::RGBA(35, 191, 0, 255) &&
							col2->renderer_.color == COLOR::RGBA(35, 191, 0, 255))
						{
							VECTOR3 back = VECTOR3(0);
							Back(*obb1, *obb2, back);
							col1->back_ += back;
						}
						col1->list_.emplace_back(col2->object_);
						col1->colliderList_.emplace_back(col2);
					}
				}
				// 平面
				else if (col2->type_ == Collider3DBase::Type::PLANE)
				{
					Collider3D::Plane* plane = static_cast<Collider3D::Plane*>(col2);
					VECTOR3 back = VECTOR3(0);
					if (HitOBBPlane(*obb1, *plane, back))
					{
						col1->back_ += back;
						col1->list_.emplace_back(col2->object_);
						col1->colliderList_.emplace_back(col2);
					}
				}
				else if (col2->type_ == Collider3DBase::Type::SPHERE)
				{
					Collider3D::Sphere* s = static_cast<Collider3D::Sphere*>(col2);
					if (HitOBBSphere(*obb1, *s))
					{
						col2->list_.emplace_back(col1->object_);
						col2->colliderList_.emplace_back(col1);
						col1->list_.emplace_back(col2->object_);
						col1->colliderList_.emplace_back(col2);
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
		for (uint j = 0; j < p2.size() - 2; j++)
		{
			// 初期化
			Vector3 cross  = { 1, 1, 1 };
			bool check = false;
			// トライアングルストリップだからな！！！！！！！！！！！！！！！！！！！！
			for (uint i = j; i < 3 + j; i++)
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
	VECTOR3 diff = s2->GetTransform().position - s1->GetTransform().position;
	float r = Half(s1->size_.x) + Half(s2->size_.x);

	return (VecLengthSq(diff) <= r * r) ? true : false;
}

// OBB同士の当たり判定
bool Collision3DManager::HitOBBs(const Collider3D::OBB& obb1, const Collider3D::OBB& obb2)
{
	// 一番長い辺を半径とし、球の当たり判定を行い負荷を減らす
	VECTOR3 diff = obb1.transform_.position - obb2.transform_.position;
	float size[2] = { 0, 0 };
	for (int i = 0; i < 3; ++i)
	{
		float len = obb1.GetLen(i);
		if (len > size[0]) { size[0] = len; }

		len = obb2.GetLen(i);
		if (len > size[1]) { size[1] = len; }
	}
	float r = size[0] + size[1];
	if (VecLengthSq(diff) > r * r) { return false; }

	// 各方向ベクトルの確保
	VECTOR3 NAe[3], NBe[3], Ae[3], Be[3];
	for (int i = 0; i < 3; ++i)
	{
		NAe[i] = obb1.GetDirect(i);
		Ae[i]  = NAe[i] * obb1.GetLen(i);
		NBe[i] = obb2.GetDirect(i);
		Be[i]  = NBe[i] * obb2.GetLen(i);
	}
	VECTOR3 interval = diff;

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

bool Collision3DManager::HitOBBSphere(const Collider3D::OBB& obb1, const Collider3D::Sphere& s2)
{
	VECTOR3 vec = VECTOR3(0);
								
	// 各軸についてはみ出た部分のベクトルを算出
	for (int i = 0; i < 3; ++i)
	{
		float l = obb1.GetLen(i);
		if (l <= 0) { continue; }
		float s = VecDot(s2.GetTransform().position - obb1.GetTransform().position, obb1.GetDirect(i)) / l;

		// sの値から、はみ出した部分があればそのベクトルを加算
		s = fabs(s);
		if (s > 1) { vec += obb1.GetDirect(i) * ((1 - s)*l); }
	}

	return (VecLength(vec) <= Half(s2.size_.x)) ? true : false;
}

// OBBと平面の当たり判定
bool Collision3DManager::HitOBBPlane(const Collider3D::OBB& obb, const Collider3D::Plane& plane, VECTOR3& length)
{
	float r = 0.0f;			// 近接距離
	auto obb1Pos = obb.GetTransform().globalPosition;

	// 平面の法線に対するOBBの射影線の長さを算出
	for (int i = 0; i < 3; ++i)
	{
		VECTOR3 direct = obb.GetDirect(i); // OBBの1つの軸ベクトル
		r += Abs(VecDot(VecNorm((direct * obb.GetLen(i))), plane.GetNormal()));
	}

	// 平面とOBBの距離を算出
	float   s = VecDot((obb1Pos - plane.GetTransform().globalPosition), plane.GetNormal());

	// 戻し距離を算出
	float l = 0;
	if (s > 0) { l = r - Abs(s); }
	else { l = r + Abs(s); }

	// 衝突判定
	if (Abs(s) - r < 0.0f)
	{
		length = plane.GetNormal() * l;
		return true;
	}

	return false;
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

// OBB同士の押し戻し処理
void Collision3DManager::Back(const Collider3D::OBB& obb1, const Collider3D::OBB& obb2, VECTOR3& length)
{
	float r = 0.0f;			// 近接距離
	auto obb2Pos = obb2.GetTransform().globalPosition;
	auto obb1Pos = obb1.GetTransform().globalPosition;

	// obb2のどの面に当たっているかを調べる
	int arrayNum = -1;
	float dot = 0;
	for (int i = 0; i < 6; ++i)
	{
		// 法線方向を取得
		VECTOR3 obb2Nor = obb2.GetDirect(i % 3);
		// 法線方向が3種類しかないので3回目以上で反転させる
		if (i > 2) { obb2Nor *= -1; }

		// obb1の一番長い辺を調べる
		float obb1MaxLen = 0;
		for (int j = 0; j < 3; ++j)
		{
			float f = obb1.GetLen(j);
			if (obb1MaxLen < f)
			{
				obb1MaxLen = f;
			}
		}

		// 進行速度
		VECTOR3 obb1VelNorm = VecNorm(obb1.GetVelocity());
		// obb1の進行方向に伸びる線分
		// 始点
		VECTOR3 segS = obb1Pos;
		// 終点
		VECTOR3 segE = obb1Pos + (obb1VelNorm * obb1MaxLen * 1.1f);
		// obb2の面上の点(位置から方向に長さをかける)
		VECTOR3 planeP = obb2Pos + (obb2Nor * obb2.GetLen(i % 3));

		// 始点から平面へのベクトル
		VECTOR3 v1 = VecNorm(segS - planeP);
		// 終点から平面へのベクトル
		VECTOR3 v2 = VecNorm(segE - planeP);

		// obb1の線分とobb2の面の当たり判定
		if (VecDot(v1, obb2Nor) * VecDot(v2, obb2Nor) <= 0)
		{
			float d = VecDot(obb1VelNorm, obb2Nor);
			// 2頂点がぶつかるとどちらも反応してしまう
			// そのため内積で進行方向に向いている面を出す
			if (d < dot)
			{
				arrayNum = i;
				dot = d;
			}
		}
	}
	if (arrayNum == -1) { return; }

	VECTOR3 obb2Nor = obb2.GetDirect(arrayNum % 3);
	if (arrayNum > 2) { obb2Nor *= -1; }

	// 平面の法線に対するOBBの射影線の長さを算出
	for (int i = 0; i < 3; ++i)
	{
		VECTOR3 direct = obb1.GetDirect(i); // OBBの1つの軸ベクトル
		r += Abs(VecDot(VecNorm((direct * obb1.GetLen(i))), obb2Nor));
	}

	// 平面とOBBの距離を算出
	obb2Pos += obb2Nor * obb2.GetLen(arrayNum % 3);
	float   s = VecDot((obb1Pos - obb2Pos), obb2Nor);

	// 戻し距離を算出
	float l = 0;
	if (s > 0) { l = r - Abs(s); }
	else { l = r + Abs(s); }

	// 衝突判定
	if (Abs(s) - r < 0.0f)
	{
		length = obb2Nor * l;
	}
}
