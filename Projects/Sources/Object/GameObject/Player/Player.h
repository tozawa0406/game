/*
 * @file		Player.h
 * @brief		プレイヤーの動き
 * @author		戸澤翔太
 * @data		2018/11/02
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <FrameWork/Systems/Camera/CameraManager.h>
#include "../GameObject.h"
#include "../../Wapon/Wapon.h"

class PlayerState;
class Player : public GameObject, public GUI
{
protected:
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
		Die,
		Standup,
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

	void Hit(int damage) override;

	/* @fn		SetWapon
	 * @brief	武器の設定
	 * @param	(wapon)		武器			*/
	inline void SetWapon(Wapon* wapon) { wapon_ = wapon; wapon->SetParent(transform_, body_, hand_); }

	// ステート時に値を参照するためのGetter
	inline Camera*			GetCamera(void)			{ return camera_;		}
	inline Wapon*			GetWapon(void)			{ return wapon_;		}

	// ステートで値を設定するためのSetter
	inline void SetPosition(const VECTOR3& position) { transform_.position = position; }

	inline Collider3D::OBB* GetCollider(void) { return collider_; }

	// デバッグ用、敵の操作中にプレイヤーを操作しない
	inline bool IsDebugCtrl(void) { return (cameraManager_ && cameraManager_->GetMainNum() != 0) ? true : false; }

private:
	bool IsDed(void);

	PlayerState*	state_;				//! プレイヤーのステート

	Wapon*			wapon_;				//! 武器
	CameraManager*	cameraManager_;		//! カメラマネージャー
	Camera*			camera_;			//! カメラ

	Collider3D::OBB* collider_;			//! 当たり判定

	const MATRIX*	body_;				//! 体のボーン行列
	const MATRIX*	hand_;				//! 右手のボーン行列
};

#endif // _PLAYER_H_
