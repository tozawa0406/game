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
	const MATRIX*		parentMtx_;
	const MATRIX*		transMtx_;
	VECTOR3				offsetPosition_;
	VECTOR3				offsetRotation_;
	VECTOR3				direction_[3];
	VECTOR3				size_;
	VECTOR3				back_;

	Object*				object_;
	ColliderRenderer	renderer_;
	std::vector<Object*> list_;
	std::vector<Collider3DBase*> colliderList_;

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

	virtual void Update(void) = 0;

	std::vector<Object*> Hit(void);
	std::vector<Collider3DBase*> HitCollider(void) { return colliderList_; }

	/* @fn		SetEnable
	 * @brief	�g�p�󋵂̐ݒ�
	 * @param	(enable)	�g�p��		*/
	inline void SetEnable(bool enable) { enable_ = enable; }

	/* @fn		IsEnable
	 * @brief	�g�p�󋵂̎擾
	 * @return	�g�p��					*/
	inline bool IsEnable(void) { return enable_; }

	inline void SetParentMtx(const MATRIX* transMtx, const MATRIX* mtx) { transMtx_ = transMtx; parentMtx_ = mtx; }
	inline void SetOffsetPosition(VECTOR3 offset) { offsetPosition_ = offset; }
	inline void SetOffsetRotation(VECTOR3 offset) { offsetRotation_ = offset; }

	Object* GetParent(void) { return object_; }

	inline COLOR GetRendererColor(void) { return renderer_.GetColor(); }
	inline void SetRendererColor(COLOR color) { renderer_.SetColor(color); }

	inline const VECTOR3& GetBack(void) { return back_; }
	inline const Transform& GetTransform(void) const { return transform_; }
	void HitStop(Object* hitObj, VECTOR3& pos, VECTOR2& velocity) { UNREFERENCED_PARAMETER(hitObj); UNREFERENCED_PARAMETER(pos); UNREFERENCED_PARAMETER(velocity); };
};

namespace Collider3D
{
	class Segment : public Collider3DBase
	{
	public:
		Segment(Object* obj);
		void Update(void) override;
		void SetDirection(VECTOR3 direction, float length) { direction_[0] = VecNorm(direction); size_ = direction_[0] * length; }
	};

	class Sphere : public Collider3DBase
	{
	public:
		Sphere(Object* obj);
		void Update(void) override;
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
		VECTOR3 velocity_;

	public:
		OBB(Object* obj);
		void Update(void) override;

		// set
		void SetSize(VECTOR3 size) { size_ = size; }
		void SetDirect(int i, VECTOR3 direct) { normaDirect_[i] = VecNorm(direct); }

		// �w�莲�ԍ��̕����x�N�g�����擾
		VECTOR3 GetDirect(int i) const { return normaDirect_[i]; }
		// �w�莲�����̒������擾
		float   GetLen(int i)	 const { return this->length_[i]; }
		VECTOR3 GetVelocity(void) const { return velocity_; }
	};
}

#endif // _COLLISION_3D_H_

