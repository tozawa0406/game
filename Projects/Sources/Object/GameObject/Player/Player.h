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
		Dive,
		KnockBack,
		KnockOut,
		Die,
		Standup,
		WalkSetup,
		Setup,
		SetupDrawn,
		SetupWait,
		SetupWalk,
		Slash_1,
		Slash_2,
		Slash_3,
		MAX
	};
	//! @def	スタミナ回復値
	static constexpr float ADD_STAMINA = 0.2f;

	/* @brief	コンストラクタ		*/
	Player(void);
	/* @brief	デストラクタ		*/
	virtual ~Player(void);


	
	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	virtual void Init(void)   override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	virtual void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	なし				*/
	virtual void Update(void) override;
	
	/* @brief	デバッグ用描画更新
	 * @param	なし
	 * @return	なし				*/
	void GuiUpdate(void) override;
	
	/* @brief	ダメージ処理
	 * @param	(damage)	ダメージ
	 * @return	なし				*/
	void Hit(int damage) override;

	/* @brief	武器の設定
	 * @param	(wapon)		武器			*/
	inline void SetWapon(Wapon* wapon) { wapon_ = wapon; wapon->SetParent(transform_, body_, hand_); }

	/* @brief	カメラ取得処理				*/
	inline Camera*	GetCamera(void)	{ return camera_;		}
	/* @brief	武器の取得処理				*/
	inline Wapon*	GetWapon(void)	{ return wapon_;		}

	/* @brief	ステートで設定				*/
	inline void SetPosition(const VECTOR3& position) { transform_.position = position; }

	/* @brief	当たり判定の取得処理		*/
	inline Collider3D::OBB* GetCollider(void) { return collider_; }

	/* @brief	デバッグ用：敵の操作中にプレイヤーを操作しない	*/
	inline bool IsDebugCtrl(void) { return (cameraManager_ && cameraManager_->GetMainNum() != 0) ? true : false; }

	/* @brief	納刀抜刀の変更				*/
	inline void SetDraw(bool isDraw) { isDraw_ = isDraw;	}
	/* @brief	納刀抜刀の取得				*/
	inline bool IsDraw(void)		 { return isDraw_;		}

	/* @brief	スタミナの取得				*/
	inline float GetStamina(void)			{ return stamina_;		}
	/* @brief	スタミナの設定				*/
	inline void  SetStamina(float stamina)	{ stamina_ = stamina;	}

private:
	/* @brief	死亡判定
	 * @sa		Update()
	 * @param	なし
	 * @return	死んでいたらtrue			*/
	bool IsDedJudge(void);

	PlayerState*	state_;				//! プレイヤーのステート
	float			stamina_;			//! スタミナ
	bool			isDraw_;			//! 抜刀フラグ

	Wapon*			wapon_;				//! 武器
	CameraManager*	cameraManager_;		//! カメラマネージャー
	Camera*			camera_;			//! カメラ

	Collider3D::OBB* collider_;			//! 当たり判定

	const MATRIX*	body_;				//! 体のボーン行列
	const MATRIX*	hand_;				//! 右手のボーン行列
};

#endif // _PLAYER_H_
