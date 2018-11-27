/*
 * @file		Collider3DBase.h
 * @brief		3D�����蔻����N���X
 * @author		���V�đ�
 * @data		2018/11/27
 */
#ifndef _COLLIDER_3D_BASE_H_
#define _COLLIDER_3D_BASE_H_

#include "../../../Define/Define.h"
#include "../ColliderRenderer.h"
#include "../../BaseManager.h"

class Object;
class Collider3DBase
{
	friend class Collision3DManager;
	friend struct ColliderRenderer;
protected:
	Systems*				systems_;
	Transform				transform_;
	const MATRIX*			parentMtx_;
	const MATRIX*			transMtx_;
	VECTOR3					offsetPosition_;
	VECTOR3					offsetRotation_;
	VECTOR3					size_;
	VECTOR3					back_;

	Object*					object_;
	ColliderRenderer		renderer_;
	std::vector<Object*>	list_;
	std::vector<Collider3DBase*> colliderList_;

	enum class Type : uint8
	{
		SEGMENT,
		SPHERE,
		AABB,
		PLANE,
		OBB
	};

private:
	Type type_;
	bool enable_;

public:
	Collider3DBase(Object* obj, Type type);
	~Collider3DBase(void);

	virtual void Init(void);
	virtual void Update(void) = 0;

	inline const std::vector<Object*>&			Hit(void)			{ return list_;			}
	inline const std::vector<Collider3DBase*>&	HitCollider(void)	{ return colliderList_; }

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
};

#endif // _COLLISION_3D_BASE_H_

