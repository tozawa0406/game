/*
 * @file		Collider3DBase.h
 * @brief		3D当たり判定基底クラス
 * @author		戸澤翔太
 * @data		2018/11/27
 */
#ifndef _COLLIDER_3D_BASE_H_
#define _COLLIDER_3D_BASE_H_

#include "../../../Define/Define.h"
#include "../ColliderRenderer.h"
#include "../../BaseManager.h"

#include <Object/ObjectTag.h>

//! @enum	当たり判定のタグ
enum class ColliderTag : int8
{
	UNKNOWN = -1,
	NORMAL = 0,
	ATTACK,
	DEFENSE,
	MAX
};

struct COLLIDER_TYPE_COLOR
{
	ColliderTag	tag;
	COLOR		color;
};

class ColliderColor
{
public:
	const COLLIDER_TYPE_COLOR list[static_cast<int>(ColliderTag::MAX)] =
	{
		{ ColliderTag::NORMAL , COLOR(1, 1, 1) },
		{ ColliderTag::ATTACK , COLOR(1, 0, 0) },
		{ ColliderTag::DEFENSE, COLOR(0, 0, 1) },
	};
};

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

	std::vector<ObjectTag>	ignoreList_;
	ColliderTag				tag_;
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
	bool isTrigger_;
	bool enable_;

public:
	Collider3DBase(Object* obj, Type type);
	~Collider3DBase(void);

	virtual void Init(void);
	virtual void Update(void) = 0;

	inline const std::vector<Object*>&			Hit(void)			{ return list_;			}
	inline const std::vector<Collider3DBase*>&	HitCollider(void)	{ return colliderList_; }

	/* @brief	無視リストの取得
	 * @param	なし
	 * @return	なし					*/
	inline const std::vector<ObjectTag>&		GetIgnoreList(void) const { return ignoreList_;	}

	/* @brief	無視リストに追加
	 * @param	(tag)	無視リストに追加するオブジェクトのタグ
	 * @reurn	なし					*/
	inline void AddIgnoreList(ObjectTag tag) { ignoreList_.emplace_back(tag); }

	/* @brief	無視リストから削除
	 * @param	(tag)	削除したいタグ
	 * @return	なし					*/
	void RemoveIgnoreList(ObjectTag tag);

	/* @brief	使用状態設定
	 * @param	(enable)	使用状態
	 * @return	なし					*/
	inline void		SetEnable(bool enable) { enable_ = enable; }

	/* @brief	使用状態取得
	 * @param	なし
	 * @return	使用状態				*/
	inline bool		IsEnable(void) const { return enable_; }

	/* @brief	トリガー設定処理
	 * @param	(trigger)	トリガー
	 * @return	なし					*/
	inline void		SetTrigger(bool trigger) { isTrigger_ = trigger; }

	/* @brief	トリガー状態取得
	 * @param	なし
	 * @return	トリガー状態			*/
	inline bool		IsTrigger(void) { return isTrigger_; }

	/* @brief	オブジェクトの取得
	 * @param	なし
	 * @return	オブジェクト			*/
	inline Object*	GetParent(void) { return object_; }

	/* @brief	オブジェクトのタグを取得
	 * @param	なし
	 * @return	オブジェクトのタグ		*/
	ObjectTag		GetParentTag(void) const;
	
	/* @brief	当たり判定の種類設定
	 * @param	(type)	当たり判定の種類
	 * @return	なし					*/
	virtual void	SetColliderTag(ColliderTag tag);

	/* @brief	当たり判定の種類取得
	 * @param	なし
	 * @return	当たり判定の種類		*/
	inline ColliderTag GetColliderTag(void) const { return tag_; }
	
	inline void SetParentMtx(const MATRIX* transMtx, const MATRIX* mtx) { transMtx_ = transMtx; parentMtx_ = mtx; }
	inline const MATRIX* GetTransMtx(void)	 const { return transMtx_; }
	inline const MATRIX* GetParentMtx(void)  const { return parentMtx_; }
	inline void SetOffsetPosition(const VECTOR3& offset) { offsetPosition_ = offset; }
	inline const VECTOR3& GetOffsetPosition(void) const { return offsetPosition_; }
	inline void SetOffsetRotation(const VECTOR3& offset) { offsetRotation_ = offset; }
	inline const VECTOR3& GetOffsetRotation(void) const { return offsetRotation_; }

	inline COLOR GetRendererColor(void) { return renderer_.GetColor(); }

	inline const VECTOR3& GetBack(void) { return back_; }
	inline const Transform& GetTransform(void) const { return transform_; }
};

#endif // _COLLISION_3D_BASE_H_

