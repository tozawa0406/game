//-----------------------------------------------------------------------------
//
//	3D�����蔻��Ǘ�[Collider3DManager.cpp]
//	Auther : ���V�đ�
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

// �_�Ɛ��̓����蔻��
bool Collision3DManager::HitPointSegment(Point p, Segment s)
{
	// ����
	Vector3 segment = Half(Vector3(s.GetEndPoint().x - s.p.x, s.GetEndPoint().y - s.p.y, s.GetEndPoint().z - s.p.z));
	// �_��������̎n�_�֌������x�N�g��
	Vector3 vec2    =  p - (s.p + segment);

	//	| v1�~v | ��0�A���� | v1 | �� | v | �Ȃ�ΏՓ˂��N�����Ă���
	if (segment.isParallel(vec2))
	{
		if (vec2.lengthSq() <= segment.lengthSq())
		{
 			return true;
		}
	}
	return false;
}

// ���Ɛ��̓����蔻��
bool Collision3DManager::HitSegments(Segment s1, Segment s2)
{
	// �x�N�g��1
	Float3 end = s1.GetEndPoint();
	Vector3 vec1 = Float3(end.x - s1.p.x, end.y - s1.p.y, end.z - s1.p.z);
	// �x�N�g��2
	end = s2.GetEndPoint();
	Vector3 vec2 = Float3(end.x - s2.p.x, end.y - s2.p.y, end.z - s2.p.z);
	// �x�N�g��1�ƃx�N�g��2�̒��_�̋���
	Vector3 vec3 = (s1.p + Half(vec1)) - (s2.p + Half(vec2));

	// �x�N�g��1�ƃx�N�g��2�̊O��
	Vector3 n1 = vec1.cross(vec2);
	// �x�N�g��1�ƍ��Ƃ̊O��
	Vector3 n2 = vec1.cross(vec3);

	// 2�ӂ����s�ł��邩
	float sq = n2.cast().lengthSq();
	// ���s�ł���Ȃ��
	if (sq < 0.01f)
	{
		// �x�N�g��1�̔����ƃx�N�g��2�̔����𑫂�
		Vector3 temp = Half(vec1.cast().abs()) + Half(vec2.cast().abs());
		// ����
		sq = vec3.cast().lengthSq();

		// �~�̓����蔻��
		if (sq < temp.lengthSq()) { return true; }
	}
	// �O�ς����s�ł���Ƃ�
	if (n1.isParallel(n2))
	{
		end = s2.GetEndPoint();
		// �x�N�g��1�̎n�_���x�N�g��2�̎n�_���I�_���傫���Ƃ�
		if (s2.p <= s1.p || end <= s1.p)
		{
			end = s1.GetEndPoint();
			// �x�N�g��1�̏I�_���x�N�g��2�̎n�_���I�_��菬�����Ƃ�
			if (end <= s2.p || end <= s2.GetEndPoint())
			{
				return true;
			}
		}
		// �x�N�g��1�̎n�_���x�N�g��2�̎n�_���I�_��菬�����Ƃ�
		else if (s1.p <= s2.p || s1.p <= end)
		{
			end = s1.GetEndPoint();
			// �x�N�g��1�̏I�_���x�N�g��2�̎n�_���I�_���傫���Ƃ�
			if (s2.p <= end || s2.GetEndPoint() <= end)
			{
				return true;
			}
		}
	}
	return false;
}

// ���ƕ��ʂ̓����蔻��
bool Collision3DManager::HitSegmentPlane(Segment s1, Point p2, Float3 n2)
{
	// ���̎n�_���畽�ʂ̈�_�܂ł̃x�N�g��
	Vector3 vec1 = s1.p - p2;
	// ���̏I�_���畽�ʂ̈�_�܂ł̃x�N�g��
	Vector3 vec2 = s1.GetEndPoint() - p2;

	// �ǂ��炩�̃x�N�g�������ς�0�ȉ��������ꍇ�ɏՓ�
	if (vec1.dot(n2) * vec2.dot(n2) <= 0)
	{
		return true;
	}
	return false;
}

// ���Ɣ|���S���̓����蔻��
bool Collision3DManager::HitSegmentBoard(Segment s1, Float3 n2, std::vector<Point> p2)
{
	if (p2.size() == 0) { return false; }
	// ���̎n�_���畽�ʂ̈�_�܂ł̃x�N�g��
	Vector3 vec1 = s1.p - p2[0];
	// ���̏I�_���畽�ʂ̈�_�܂ł̃x�N�g��
	Vector3 vec2 = s1.GetEndPoint() - p2[0];

	// �ǂ��炩�̃x�N�g�������ς�0�ȉ��������ꍇ�ɕ��ʂ��ђ�
	if (vec1.dot(n2) * vec2.dot(n2) <= 0)
	{
		// �_���畽�ʂ܂ł̋���
		// �n�_
		Vector3 v1 = s1.p - p2[0];
		float d1 = fabs(n2.GetNorm().dot(v1));
		// �I�_
		Vector3 v2 = s1.GetEndPoint() - p2[0];
		float d2 = fabs(n2.GetNorm().dot(v2));

		// ������
		float a = d1 / (d1 + d2);
		// �ŏ��̒��_����ђʓ_�܂ł̋���
		Vector3 vec3 = (1 - a) * vec1.cast() + a * vec2.cast();

		// �|���S�����ʂ܂ł̋��������������Z�o���Ċђʓ_�̍��W���m��
		Point p3 = p2[0] + vec3.cast();
			
		// �|���S�������Ɋђʓ_���܂܂�邩���`�F�b�N
		bool hit = false;
		// �g���C�A���O���X�g���b�v�̂����œ��郋�[�v
		for (UINT j = 0; j < p2.size() - 2; j++)
		{
			// ������
			Vector3 cross  = { 1, 1, 1 };
			bool check = false;
			// �g���C�A���O���X�g���b�v������ȁI�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I
			for (UINT i = j; i < 3 + j; i++)
			{
				Vector3 vector1;
				// ���̒��_�ւ̃x�N�g��
				if (i < j + 2) { vector1 = p2[i + 1] - p2[i]; }
				// �Ō�͍ŏ��̒��_�ւ̃x�N�g��
				else           { vector1 = p2[j]     - p2[i]; }
				// �ђʓ_���猻�݂̒��_�ւ̃x�N�g��
				Vector3 vector2 = p2[i] - p3;

				// ��L��̃x�N�g���̊O��
				Vector3 c = vector1.cross(vector2).GetNorm();
				// �O�ς����ƈႦ�Ίђʓ_�̓|���S���̊O
				if (cross != c)
				{
					// �O�ς��������̏�ԂłȂ���Γ������ĂȂ�
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

// ���Ɠ��̓����蔻��
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

// ���Ƌ��̓����蔻��
bool Collision3DManager::HitSpheres(const Collider3DBase* s1, const Collider3DBase* s2)
{
	Point diff = Float3(s2->transform_.position.x - s1->transform_.position.x, s2->transform_.position.y - s1->transform_.position.y, s2->transform_.position.z - s1->transform_.position.z);
	float r = Half(s1->size_.x) + Half(s2->size_.x);

	return (diff.lengthSq() <= r * r) ? true : false;
}

// OBB���m�̓����蔻��
bool Collision3DManager::HitOBBs(const Collider3D::OBB& obb1, const Collider3D::OBB& obb2)
{
	// �e�����x�N�g���̊m��
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
		// ������ : Ae
		if (!SeparationA(Ae[i], NAe[i], Be[0], Be[1], Be[2], interval)) { return false; }
		// ������ : Be
		if (!SeparationB(Be[i], NBe[i], Ae[0], Ae[1], Ae[2], interval)) { return false; }
	}

	// ������ : C11
	if (!SeparationC(NAe[0], NBe[0], Ae[1], Ae[2], Be[1], Be[2], interval)) { return false; }
	// ������ : C12
	if (!SeparationC(NAe[0], NBe[1], Ae[1], Ae[2], Be[0], Be[2], interval)) { return false; }
	// ������ : C13
	if (!SeparationC(NAe[0], NBe[2], Ae[1], Ae[2], Be[0], Be[1], interval)) { return false; }

	// ������ : C21
	if (!SeparationC(NAe[1], NBe[0], Ae[0], Ae[2], Be[1], Be[2], interval)) { return false; }
	// ������ : C22
	if (!SeparationC(NAe[1], NBe[1], Ae[0], Ae[2], Be[0], Be[2], interval)) { return false; }
	// ������ : C23
	if (!SeparationC(NAe[1], NBe[2], Ae[0], Ae[2], Be[0], Be[1], interval)) { return false; }

	// ������ : C31
	if (!SeparationC(NAe[2], NBe[0], Ae[0], Ae[1], Be[1], Be[2], interval)) { return false; }
	// ������ : C32
	if (!SeparationC(NAe[2], NBe[1], Ae[0], Ae[1], Be[0], Be[2], interval)) { return false; }
	// ������ : C33
	if (!SeparationC(NAe[2], NBe[2], Ae[0], Ae[1], Be[0], Be[1], interval)) { return false; }

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}

float Collision3DManager::LenSqAABBToPoint(AABB &box, Point &p)
{
	FLOAT SqLen = 0;   // �����ׂ̂���̒l���i�[
	int i;
	for (i = 0; i<3; i++)
	{
		// �e���œ_���ŏ��l�ȉ��������͍ő�l�ȏ�Ȃ�΁A�����l��
		if (p.Get(i) < box.GetMin(i))  // i = 0 = X�A1 = Y�A2 = Z
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

	for (int i = 0; i < 3; i++)	// �e���ɂ��Ă͂ݏo�������̃x�N�g�����Z�o
	{
		FLOAT L = obb.GetLen(i);
		if (L <= 0) { continue; }  // L=0�͌v�Z�ł��Ȃ�
		float s = Vector3(p - obb.GetPos()).dot(obb.GetDirect(i)) / L;

		// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
		s = fabs(s);
		if (s > 1)
		{
			vec = vec + (1 - s) * L * obb.GetDirect(i);   // �͂ݏo���������̃x�N�g���Z�o
		}
	}
	return vec.lengthSq();
}

float Collision3DManager::LenOBBToPoint(OBB &obb, Point &p)
{
	return sqrt(LenSqOBBToPoint(obb, p));
}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
float Collision3DManager::LenSegOnSeparateAxis(VECTOR3& Sep, VECTOR3& e1, VECTOR3& e2, VECTOR3* e3)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	float r1 = fabs(VecDot(Sep, e1));
	float r2 = fabs(VecDot(Sep, e2));
	float r3 = e3 ? (fabs(VecDot(Sep, *e3))) : 0;
	return r1 + r2 + r3;
}

// ������A
bool Collision3DManager::SeparationA(VECTOR3& Ae, VECTOR3& NAe, VECTOR3& Be1, VECTOR3& Be2, VECTOR3& Be3, VECTOR3& interval)
{
	float rA = VecLength(Ae);
	float rB = LenSegOnSeparateAxis(NAe, Be1, Be2, &Be3);
	float L = fabs(VecDot(interval, NAe));
	if (L > rA + rB)
	{
		return false; // �Փ˂��Ă��Ȃ�
	}
	return true;
}

// ������B
bool Collision3DManager::SeparationB(VECTOR3& Be, VECTOR3& NBe, VECTOR3& Ae1, VECTOR3& Ae2, VECTOR3& Ae3, VECTOR3& interval)
{
	float rA = LenSegOnSeparateAxis(NBe, Ae1, Ae2, &Ae3);
	float rB = VecLength(Be);
	float L = fabs(VecDot(interval, NBe));
	if (L > rA + rB)
	{
		return false;	// �Փ˂��Ă��Ȃ�
	}
	return true;
}

// ������C
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
