/*
 * @file		Dragon.h
 * @brief		敵の動き
 * @author		戸澤翔太
 * @data		2018/11/02
 */
#ifndef _DRAGON_H_
#define _DRAGON_H_

#include <FrameWork/Systems/Camera/CameraManager.h>
#include "../../GameObject.h"
#include "../MonsterAttack.h"
#include "DragonMoveController.h"

struct BONE_COLLISION
{
	string	boneName;
	VECTOR3 offsetPosition;
	VECTOR3 offsetRotation;
	VECTOR3 size;
};

class Dragon : public GameObject, public GUI
{
	//! @enum	当たり判定
	enum class Collision : uint8
	{
		BODY = 0,
		HEAD,
		NECK1,
		NECK2,
		WING_LL,
		WING_LC,
		WING_LR,
		WING_L_CLAW,
		WING_RL,
		WING_RC,
		WING_RR,
		WING_R_CLAW,
		ARM_L_UP,
		ARM_L_UP2,
		ARM_L_DOWN,
		ARM_R_UP,
		ARM_R_UP2,
		ARM_R_DOWN,
		LEGS_L_UP,
		LEGS_L_UP2,
		LEGS_L_DOWN,
		LEGS_R_UP,
		LEGS_R_UP2,
		LEGS_R_DOWN,
		TAIL5,
		TAIL4,
		TAIL3,
		TAIL2,
		TAIL1,
		MAX
	};

	//! @enum	攻撃パターン
	enum class AttackPattern : uint8
	{
		SCREAM = 0,
		BITE,
		WING_ATTACK,
		TAIL_ATTACK,
		TAKE_OFF,
		HIT,
		RUSH,

		MAX
	};
public:
	//! @enum	アニメーション
	enum class Animation : uint8
	{
		WAIT1 = 0,
		WAIT2,
		WALK,
		RUN,
		SCREAM,
		HIT,
		DIE,
		BITE,
		WING_ATTACK,
		TAIL_ATTACK,
		TAKE_OFF,
		FLY_IDLE,

		MAX
	};
	//! @def	移動速度
	static constexpr float MOVE_SPEED = 0.03f;
	//! @def	走りフラグ
	static constexpr uint IS_DASH = 0x0001;

	/* @brief	コンストラクタ		*/
	Dragon(void);
	/* @brief	デストラクタ		*/
	~Dragon(void);


	
	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void)	  override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	なし				*/
	void Update(void) override;
	
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし				*/
	void GuiUpdate(void) override;
	
	/* @brief	ダメージ処理
	 * @param	(damage)	ダメージ
	 * @return	なし				*/
	void Hit(int damage) override;

private:
	/* @brief	当たり判定生成処理
	 * @sa		Init
	 * @param	なし
	 * @return	なし				*/
	void CreateCollision(void);
	
	/* @brief	デバッグ用操作
	 * @sa		Update()
	 * @param	なし
	 * @return	実行したらtrue		*/
	bool DebugInput(void);
	
	/* @brief	被ダメージ処理
	 * @sa		Update()
	 * @param	なし
	 * @return	なし				*/
	bool TakenDamage(void);
	
	/* @brief	コリジョンの設定
	 * @param	(arrayNum)	生成するコリジョンの配列番号
	 * @param	(boneName)	ボーンの名前
	 * @param	(offset)	オフセット
	 * @param	(model)		モデルデータ
	 * @return	なし				*/
	void SetCollision(int arrayNum, const BONE_COLLISION& offset, const MODEL& model);

	//! フラグ
	uint				flag_;
	//! 当たり判定
	Collider3D::OBB*	collision_[static_cast<uint8>(Collision::MAX)];
	Collider3D::OBB*	collisionDefense_[static_cast<uint8>(Collision::MAX)];

	//! 攻撃処理のインスタンス
	MonsterAttack*			attack_[static_cast<int>(AttackPattern::MAX)];
	//! 現在の攻撃処理
	MonsterAttack*			currentAttack_;
	//! 攻撃処理操作
	DragonMoveController*	moveController_;

	int				accumulation_;		//! 被ダメージ蓄積値

	CameraManager*	cameraManager_;		//! カメラマネージャー
	Camera*			camera_;			//! カメラ

	bool			debugMove_;			//! デバッグ用コントロールフラグ

	static const BONE_COLLISION BONE_COLLISION_OFFSET[static_cast<int>(Collision::MAX)];
	static int tint;
};

#endif // _DRAGON_H_
