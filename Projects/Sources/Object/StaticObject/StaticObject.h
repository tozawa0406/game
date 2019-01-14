/*
 * @file		StaticObject.h
 * @brief		�����Ȃ��I�u�W�F�N�g
 * @author		���V�đ�
 * @data		2019/01/15
 */
#ifndef _STATIC_OBJECT_H_
#define _STATIC_OBJECT_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class StaticObject : public Object
{
public:
	/* @brief	�R���X�g���N�^
	 * @param	(model)	���f��		*/
	StaticObject(uint8 model);
	/* @brief	�f�X�g���N�^		*/
	~StaticObject(void);
	


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
	 * @param	(trans)		�p��	*/
	inline void SetTransform(Transform trans) { transform_ = trans; }

	/* @brief	�����蔻��̐ݒ�
	 * @param	(position)	�I�t�Z�b�g�ʒu
	 * @param	(rotation)	�I�t�Z�b�g��]
	 * @param	(size)		�����蔻��̑傫��		*/
	void SetColliderOffset(VECTOR3 position, VECTOR3 rotation, VECTOR3 size);

private:
	//! �����蔻��
	Collider3D::OBB*	collider_;
	//! ���b�V��
	MeshRenderer		mesh_;
	//! ���f��
	uint8				model_;
};

#endif // _STATIC_OBJECT_H_
