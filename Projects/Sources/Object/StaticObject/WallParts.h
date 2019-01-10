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
	WallParts(void);
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
	
	/* @brief	�p���̐ݒ�
	 * @param	(trans)		3D�p��				*/
	inline void SetTransform(const Transform& trans) { transform_ = trans; }

	/* @brief	���f���̐ݒ�
	 * @param	(model)		���f���̔ԍ�		*/
	inline void SetModelNum(Model::Game model) { mesh_.Init(Systems::Instance(), static_cast<int>(model), &transform_); }

	/* @brief	�����蔻��̑傫���w��
	 * @param	(size)		�傫��				*/
	inline void SetColliderSize(const VECTOR3 size) { if (collider_) { collider_->SetSize(size); } }
	
	/* @brief	�����蔻��̃I�t�Z�b�g�ݒ�
	 * @param	(trans)		3D�p��				*/
	void SetColliderOffset(const VECTOR3& position, const VECTOR3& rotation);

private:
	//! �����蔻��
	Collider3D::OBB*	collider_;
	//! ���b�V��
	MeshRenderer		mesh_;
};

#endif // _WALL_A_H_
