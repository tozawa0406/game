/*
 * @file		GameObject.h
 * @brief		ダメージ、移動処理のあるゲーム内オブジェクト
 * @author		戸澤翔太
 * @data		2018/11/13
 */
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>

struct MESH_ANIMATION
{
	MeshRenderer	mesh;				//! メッシュ
	int				animation;			//! アニメーション
	float			animSpeed;			//! アニメーション再生速度
};

class GameObject : public Object
{
public:
	GameObject(Object::Tag tag);
	virtual ~GameObject(void);

	virtual void Hit(int damage) = 0;

	/* @fn		GetMeshAnim
	 * @brief	派生先のステートや行動で変更する際に使用
				それ以外で使うな							*/
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
