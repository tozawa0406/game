/*
 * @file		WallParts.h
 * @brief		�ǂ̏����ȃI�u�W�F�N�g
 * @author		���V�đ�
 * @data		2019/01/10
 */
#ifndef _WALL_PARTS_H_
#define _WALL_PARTS_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class WallParts : public Object
{
public:
	/* @brief	�R���X�g���N�^
	 * @param	(transform)		�����p��
	 * @param	(model)			���f��		*/
	WallParts(Model::Game model);
	/* @brief	�f�X�g���N�^		*/
	~WallParts(void);
	


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
	//! �����蔻��
	Collider3D::OBB*	collider_;
	//! ���b�V��
	MeshRenderer		mesh_;
	//! ���f��
	Model::Game model_;
};

#endif // _WALL_A_H_
