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
	/* @brief	コンストラクタ		*/
	GameObject(ObjectTag tag);
	/* @brief	デストラクタ		*/
	virtual ~GameObject(void);

	virtual void Hit(int damage) = 0;

	/* @brief	派生先のステートや行動で変更する際に使用
				それ以外で使うな							*/
	inline MESH_ANIMATION& GetMeshAnimation(void) { return meshAnim_; }

	/* @brief	移動速度取得		*/
	inline const VECTOR3&  GetVelocity(void)					{ return velocity_;		}
	/* @brief	移動速度の設定		*/
	inline void			   SetVelocity(const VECTOR3& velocity)	{ velocity_ = velocity; }

	/* @brief	前ベクトル			*/
	inline const VECTOR3& GetFront(void) { return front_; }
	/* @brief	右ベクトル			*/
	inline const VECTOR3& GetRight(void) { return right_; }

	/* @brief	アニメーション終了フラグ		*/
	inline bool IsEndAnim(void) { return isEndAnim_; }

	/* @brief	現在ライフ			*/
	inline int  GetLife(void) { return life_; }

	/* @brief	死亡判定			*/
	inline bool IsDed(void) { return ded_; }

private:
	/* @brief	前ベクトルの生成
	 * @sa		Move()
	 * @param	なし
	 * @return	なし				*/
	void CreateFrontVector(void);

protected:
	/* @brief	移動時の回転処理
	 * @sa		Update()
	 * @param	なし
	 * @return	なし				*/
	void Move(void);
	
	/* @brief	接地判定
	 * @sa		Update()
	 * @param	なし
	 * @return	なし				*/
	void OnGround(void);

	void CreateDefenseCollider(const Collider3D::OBB& normal, Collider3D::OBB* copy);

	int life_;						//! ライフ

	MESH_ANIMATION	meshAnim_;		//! メッシュとアニメーション情報
	bool			ded_;			//! 死亡フラグ
	bool			isEndAnim_;		//! アニメーション終了フラグ
	VECTOR3			velocity_;		//! 速度
	VECTOR3			front_;			//! 前ベクトル
	VECTOR3			right_;			//! 右ベクトル
};

#endif // _GAME_OBJECT_H_
