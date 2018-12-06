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
	static constexpr int MESH_NUM = 2;
public:
	WallA(const VECTOR3& position, const VECTOR3& rotation);
	~WallA(void);

	void Init(void) override;
	void Uninit(void) override;

	/* @brief	�X�V����(�����Ȃ����߂Ȃ�)		*/
	void Update(void) override {}

private:
	Collider3D::Plane* collider_;	//! �����蔻��

	//! ���b�V��
	MeshRenderer	mesh_[MESH_NUM];
	//! ���b�V���̎p��
	Transform		transformMesh_[MESH_NUM];
};

#endif // _WALL_A_H_
