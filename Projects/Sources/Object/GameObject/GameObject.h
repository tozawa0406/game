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
	GameObject(Object::Tag tag);
	virtual ~GameObject(void);

	virtual void Hit(int damage) = 0;

	/* @fn		GetMeshAnim
	 * @brief	�h����̃X�e�[�g��s���ŕύX����ۂɎg�p
				����ȊO�Ŏg����							*/
	inline MESH_ANIMATION& GetMeshAnimation(void) { return meshAnim_; }

	inline const VECTOR3&  GetVelocity(void)					{ return velocity_;		}
	inline void			   SetVelocity(const VECTOR3& velocity)	{ velocity_ = velocity; }

	inline const VECTOR3& GetFront(void) { return front_; }
	inline const VECTOR3& GetRight(void) { return right_; }

	inline bool IsEndAnim(void) { return isEndAnim_; }

	inline int  GetLife(void) { return life_; }

private:
	void CreateFrontVector(void);

protected:
	void Move(void);
	void OnGround(void);

	int life_;

	MESH_ANIMATION	meshAnim_;
	bool			ded_;
	bool			isEndAnim_;
	VECTOR3			velocity_;
	VECTOR3			front_;
	VECTOR3			right_;
};

#endif // _GAME_OBJECT_H_
