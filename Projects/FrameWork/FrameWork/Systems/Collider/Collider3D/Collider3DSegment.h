/*
 * @file		Collider3DSegment.h
 * @brief		��
 * @author		���V�đ�
 * @data		2018/11/27
 */
#ifndef _COLLIDER_3D_SEGMENT_H_
#define _COLLIDER_3D_SEGMENT_H_

#include "Collider3DBase.h"

namespace Collider3D
{
	class Segment : public Collider3DBase
	{
	public:
		Segment(Object* obj);
		void Update(void) override;
	};
}

#endif // _COLLISION_3D_SEGMENT_H_

