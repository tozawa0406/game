/*
 * @file		Collider3DPlane.h
 * @brief		•½–Ê
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/27
 */
#ifndef _COLLIDER_3D_PLANE_H_
#define _COLLIDER_3D_PLANE_H_

#include "Collider3DBase.h"

namespace Collider3D
{
	class Plane : public Collider3DBase
	{
	protected:
		VECTOR3 normal_;

	public:
		Plane(Object* obj);
		void Update(void) override;

		void SetNormal(const VECTOR3& normal);
		inline const VECTOR3& GetNormal(void) const { return normal_; }
	};
}

#endif // _COLLISION_3D_PLANE_H_
