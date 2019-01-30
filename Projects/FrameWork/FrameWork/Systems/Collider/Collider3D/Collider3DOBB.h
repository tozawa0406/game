/*
 * @file		Collider3DOBB.h
 * @brief		OBB
 * @author		���V�đ�
 * @data		2018/11/27
 */
#ifndef _COLLIDER_3D_OBB_H_
#define _COLLIDER_3D_OBB_H_

#include "Collider3DBase.h"

namespace Collider3D
{
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
		void SetSize(const VECTOR3& size) { size_ = size; }
		void SetDirect(int i, const VECTOR3& direct) { normaDirect_[i] = VecNorm(direct); }

		inline const VECTOR3& GetSize(void) const { return size_; }
		// �w�莲�ԍ��̕����x�N�g�����擾
		inline const VECTOR3& GetDirect(int i)  const { return normaDirect_[i]; }
		// �w�莲�����̒������擾
					 float    GetLen(int i)	    const { return length_[i];		}
		inline const VECTOR3& GetVelocity(void) const { return velocity_;		}
	};
}

#endif // _COLLISION_3D_OBB_H_
