/*
 * @file		CampWallParts.h
 * @brief		�L�����v�̕ǂ̈��
 * @author		���V�đ�
 * @data		2018/11/27
 */
#ifndef _CAMP_WALL_PARTS_H_
#define _CAMP_WALL_PARTS_H_

#include "WallParts.h"

class CampWallParts : public WallParts
{
public:
	/* @brief	�R���X�g���N�^
	 * @param	(position)		�����ʒu
	 * @param	(rotation)		������]�p		*/
	CampWallParts(const VECTOR3& position, const VECTOR3& rotation);
	/* @brief	�f�X�g���N�^		*/
	~CampWallParts(void);
	


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
	Collider3D::OBB*	entranceCollider_[2];	//! �����p�̓����蔻��
};

#endif // _CAMP_WALL_PARTS_H_
