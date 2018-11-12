/*
 * @file		Player.h
 * @brief		プレイヤーの動き
 * @author		戸澤翔太
 * @data		2018/11/02
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <FrameWork/Systems/Camera/CameraManager.h>
#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

#include "../Wapon/Wapon.h"

struct MESH_ANIMATION
{
	MeshRenderer	mesh;				//! メッシュ
	int				animation;			//! アニメーション
	float			animSpeed;			//! アニメーション再生速度
};

class PlayerState;
class Player : public Object, public GUI
{
protected:
	//! @def	抜刀フラグ
	static constexpr uint IS_DRAWN			= 0x0001;
	//! @def	納刀抜刀遷移フラグ
	static constexpr uint IS_SETUP			= 0x0002;
	//! @def	回避時フラグ
	static constexpr uint IS_AVOIDANCE		= 0x0004;
	//! @def	攻撃フラグ
	static constexpr uint IS_ATTACK			= 0x0008;
	//! @def	次の攻撃フラグ
	static constexpr uint IS_NEXT_SLASH		= 0x0010;
	//! @def	アニメーション終了フラグ
	static constexpr uint IS_END_ANIMATION	= 0x0020;


	//! @def	アニメーションの速度
	static constexpr float ANIMATION_DEFAULT = 0.75f;

public:
	//! @enum	アニメーション
	enum class Animation
	{
		Wait = 0,
		WaitTime1,
		WaitTime2,
		Walk,
		Run,
		Roll,
		KnockBack,
		KnockOut,
		Setup,
		SetupWait,
		SetupWalk,
		Slash_1,
		Slash_2,
		Slash_3,
		MAX
	};

	Player(void);
	virtual ~Player(void);

	virtual void Init(void)   override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	void GuiUpdate(void) override;

	/* @fn		SetWapon
	 * @brief	武器の設定
	 * @param	(wapon)		武器			*/
	inline void SetWapon(Wapon* wapon) { wapon_ = wapon; wapon->SetParent(transform_, body_, hand_); }

	// ステート時に値を参照するためのGetter
	inline MESH_ANIMATION&	GetMeshAnimation(void)	{ return meshAnim_;		}
	inline Camera*			GetCamera(void)			{ return camera_;		}
	inline const VECTOR3&	GetVelocity(void)		{ return velocity_;		}
	inline bool				IsEndAnim(void)			{ return isEndAnim_;	}
	inline Wapon*			GetWapon(void)			{ return wapon_;		}
	inline const VECTOR3&	GetFront(void)			{ return front_; }

	// ステートで値を設定するためのSetter
	inline void SetVelocity(const VECTOR3& velocity) { velocity_ = velocity; }

protected:
	PlayerState*	state_;				//! プレイヤーのステート

	MESH_ANIMATION	meshAnim_;			//! メッシュとアニメーション情報
	VECTOR3			velocity_;			//! 移動速度
	bool			isEndAnim_;			//! アニメーションの終了判定
	VECTOR3			front_;				//! 前ベクトル
	Wapon*			wapon_;				//! 武器
	CameraManager*	cameraManager_;		//! カメラマネージャー
	Camera*			camera_;			//! カメラ

	Collider3D::OBB* collider_;			//! 当たり判定


private:
	void Move(void);
	void CreateFrontVector(void);
	void OnGround(void);

	const MATRIX*	body_;				//! 体のボーン行列
	const MATRIX*	hand_;				//! 右手のボーン行列
};

#endif // _PLAYER_H_
