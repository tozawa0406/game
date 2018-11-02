//-----------------------------------------------------------------------------
//
//	3D�����蔻��[Collider3D.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _COLLIDER_3D_H_
#define _COLLIDER_3D_H_

#include "../../Define/Define.h"
#include "ColliderRenderer.h"
#include "../BaseManager.h"

#define _OX_EPSILON_	0.000001f	// �덷

//-----------------------------------------------------------------------------
//	�N���X�錾
//-----------------------------------------------------------------------------
class Object;
class Collider3DBase
{
	friend class Collision3DManager;
	friend struct ColliderRenderer;
protected:
	Systems*			systems_;
	Transform			transform_;
	VECTOR3				offset_;
	VECTOR3				direction_[3];
	VECTOR3				size_;

	Object*				object_;
	ColliderRenderer	renderer_;
	std::vector<Object*> list_;

	enum Type
	{
		SEGMENT,
		SPHERE,
		AABB,
		OBB
	};

private:
	Type type_;
	bool enable_;

public:
	Collider3DBase(Object* obj, Type type);
	~Collider3DBase(void);

	std::vector<Object*> Hit(void);

	/* @fn		SetEnable
	 * @brief	�g�p�󋵂̐ݒ�
	 * @param	(enable)	�g�p��		*/
	inline void SetEnable(bool enable) { enable_ = enable; }

	/* @fn		IsEnable
	 * @brief	�g�p�󋵂̎擾
	 * @return	�g�p��					*/
	inline bool IsEnable(void) { return enable_; }

	void HitStop(Object* hitObj, VECTOR3& pos, VECTOR2& velocity) { UNREFERENCED_PARAMETER(hitObj); UNREFERENCED_PARAMETER(pos); UNREFERENCED_PARAMETER(velocity); };
};

namespace Collider3D
{
	class Segment : public Collider3DBase
	{
	public:
		Segment(Object* obj);
		void Update(void);
		void SetDirection(VECTOR3 direction, float length) { direction_[0] = VecNorm(direction); size_ = direction_[0] * length; }
	};

	class Sphere : public Collider3DBase
	{
	public:
		Sphere(Object* obj);
		void Update(void);
		void SetOffset(VECTOR3 offset) { offset_ = offset; }
		void SetSize(float d) { size_ = VECTOR3(d, d, d); }
	private:
		Transform			transform2_;
		ColliderRenderer	renderer2_;
	};

	class OBB : public Collider3DBase
	{
	protected:
		VECTOR3 normaDirect_[3];		// �����x�N�g��
		float   length_[3];				// �e�������̒���

	public:
		OBB(Object* obj);
		void Update(void);

		// set
		void SetSize(VECTOR3 size) { size_ = size; }
		void SetOffset(VECTOR3 offset) { offset_ = offset; }
		void SetDirect(int i, VECTOR3 direct) { normaDirect_[i] = VecNorm(direct); }

		// �w�莲�ԍ��̕����x�N�g�����擾
		VECTOR3 GetDirect(int i) const { return normaDirect_[i]; }
		// �w�莲�����̒������擾
		float   GetLen(int i)	 const { return this->length_[i]; }
	};
}

#endif // _COLLISION_3D_H_

