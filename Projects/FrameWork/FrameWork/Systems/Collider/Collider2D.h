//-----------------------------------------------------------------------------
//
//	2DìñÇΩÇËîªíË[Collider2D.h]
//	Auther : åÀ‡V„ƒëæ
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _COLLIDER_2D_H_
#define _COLLIDER_2D_H_

#include "../../Define/Define.h"
#include "ColliderRenderer.h"
#include "../BaseManager.h"
#include "../Renderer/Sprite/CanvasRenderer.h"

//-----------------------------------------------------------------------------
//	ÉNÉâÉXêÈåæ
//-----------------------------------------------------------------------------
class Object;
class Collider2DBase
{
	friend class Collision2DManager;
	friend struct ColliderRenderer;
protected:
	Systems*				systems_;
	const Transform*		transform_;
	VECTOR3					position_;
	VECTOR3					offset_;
	VECTOR2					size_;
	Object*					object_;
	ColliderRenderer		renderer_;
	std::vector<Object*>	list_;

	enum Type
	{
		CIRCLE,
		RECTANGLE
	};

private:	
	Type		type_;
	bool		enable_;

public:
	Collider2DBase(Object* obj, Type type);
	~Collider2DBase(void);

	void SetOffset(VECTOR3& offset) { offset_ = offset; }
	void SetEnable(bool enable)		{ enable_ = enable; }

	std::vector<Object*> Hit(void);
	void HitStop(Object* hitObj, VECTOR3& pos, VECTOR2& velocity);
};

namespace Collider2D
{
	class Circle : public Collider2DBase
	{
	public:
		Circle(Object* obj);
		void Update(void);
		void SetSize(float d) { size_.x = d; }
	};

	class Rectangle : public Collider2DBase
	{
	public:
		Rectangle(Object* obj);
		void Update(void);
		void SetSize(VECTOR2 size) { size_ = size; }
	};
};

#endif // _COLLEDER_2D_H_

