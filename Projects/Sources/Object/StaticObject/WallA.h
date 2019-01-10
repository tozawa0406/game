/*
 * @file		WallA.h
 * @brief		��
 * @author		���V�đ�
 * @data		2018/11/27
 */
#ifndef _WALL_A_H_
#define _WALL_A_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class WallA : public Object
{
	//! @def	1�̕ǂ�2�̃��b�V�����g�p
	static constexpr int MESH_NUM = 2;
public:
	/* @brief	�R���X�g���N�^
	 * @param	(position)		�����ʒu
	 * @param	(rotation)		������]�p		*/
	WallA(const VECTOR3& position, const VECTOR3& rotation);
	/* @brief	�f�X�g���N�^		*/
	~WallA(void);
	


	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;

	/* @brief	�X�V����(�����Ȃ����߂Ȃ�)		*/
	void Update(void) override {}

private:
	Collider3D::Plane*	collider_;				//! �����蔻��
	Collider3D::OBB*	entranceCollider_[2];	//! �����p�̓����蔻��

	//! ���b�V��
	MeshRenderer	mesh_[MESH_NUM];
	//! ���b�V���̎p��
	Transform		transformMesh_[MESH_NUM];
};

#endif // _WALL_A_H_
