/*
 * @file		PlayerMove.h
 * @brief		プレイヤーの動き
 * @author		戸澤翔太
 * @data		2018/11/02
 */
#ifndef _PLAYER_MOVE_H_
#define _PLAYER_MOVE_H_

#include <FrameWork/Systems/Camera/CameraManager.h>
#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

#include "../Wapon/Wapon.h"

class PlayerMove : public Object, public GUI
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

	//! @enum	アニメーション
	enum class Animation
	{
		Wait = 0,
		WaitTime1,
		WaitTime2,
		Walk,
		Run,
		Roll,
		Setup,
		SetupWait,
		SetupWalk,
		Slash_1,
		Slash_2,
		Slash_3,
		MAX
	};

public:
	PlayerMove(void);
	virtual ~PlayerMove(void);

	virtual void Init(void)   override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;

	void GuiUpdate(void) override;

	/* @fn		SetWapon
	 * @brief	武器の設定
	 * @param	(wapon)		武器			*/
	inline void SetWapon(Wapon* wapon) { wapon_ = wapon; wapon->SetParent(transform_, body_, hand_); }

protected:

	float			animCnt_;			//! アニメーション再生速度
	VECTOR3			velocity_;			//! 移動速度
	float			inputDash_;			//! ダッシュ入力
	VECTOR2			inputDir_;			//! 入力方向
	VECTOR3			avoidanceDir_;		//! 回避方向
	VECTOR3			front_;				//! 前ベクトル
	Wapon*			wapon_;				//! 武器
	MeshRenderer	mesh_;				//! メッシュ
	uint			flag_;				//! フラグの管理
	Animation		animation_;			//! アニメーション
	Camera*			camera_;			//! カメラ

	Collider3D::OBB* collider_;			//! 当たり判定


private:
	void StandBy(void);
	void Move(void);
	void CreateFrontVector(void);
	void OnGround(void);

	const MATRIX*	body_;				//! 体のボーン行列
	const MATRIX*	hand_;				//! 右手のボーン行列
	CameraManager*	cameraManager_;		//! カメラマネージャー
	int				waitTime_;			//! 待機待ち時間
};

#endif // _PLAYER_MOVE_H_
