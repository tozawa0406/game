/*
 * @file		GameObject.h
 * @brief		�_���[�W�A�ړ������̂���Q�[�����I�u�W�F�N�g
 * @author		���V�đ�
 * @data		2018/11/13
 */
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>

struct MESH_ANIMATION
{
	MeshRenderer	mesh;				//! ���b�V��
	int				animation;			//! �A�j���[�V����
	float			animSpeed;			//! �A�j���[�V�����Đ����x
};

class GameObject : public Object
{
public:
	/* @brief	�R���X�g���N�^		*/
	GameObject(ObjectTag tag);
	/* @brief	�f�X�g���N�^		*/
	virtual ~GameObject(void);

	virtual void Hit(int damage) = 0;

	/* @brief	�h����̃X�e�[�g��s���ŕύX����ۂɎg�p
				����ȊO�Ŏg����							*/
	inline MESH_ANIMATION& GetMeshAnimation(void) { return meshAnim_; }

	/* @brief	�ړ����x�擾		*/
	inline const VECTOR3&  GetVelocity(void)					{ return velocity_;		}
	/* @brief	�ړ����x�̐ݒ�		*/
	inline void			   SetVelocity(const VECTOR3& velocity)	{ velocity_ = velocity; }

	/* @brief	�O�x�N�g��			*/
	inline const VECTOR3& GetFront(void) { return front_; }
	/* @brief	�E�x�N�g��			*/
	inline const VECTOR3& GetRight(void) { return right_; }

	/* @brief	�A�j���[�V�����I���t���O		*/
	inline bool IsEndAnim(void) { return isEndAnim_; }

	/* @brief	���݃��C�t			*/
	inline int  GetLife(void) { return life_; }

	/* @brief	���S����			*/
	inline bool IsDed(void) { return ded_; }

private:
	/* @brief	�O�x�N�g���̐���
	 * @sa		Move()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void CreateFrontVector(void);

protected:
	/* @brief	�ړ����̉�]����
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Move(void);
	
	/* @brief	�ڒn����
	 * @sa		Update()
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void OnGround(void);

	int life_;						//! ���C�t

	MESH_ANIMATION	meshAnim_;		//! ���b�V���ƃA�j���[�V�������
	bool			ded_;			//! ���S�t���O
	bool			isEndAnim_;		//! �A�j���[�V�����I���t���O
	VECTOR3			velocity_;		//! ���x
	VECTOR3			front_;			//! �O�x�N�g��
	VECTOR3			right_;			//! �E�x�N�g��
};

#endif // _GAME_OBJECT_H_
