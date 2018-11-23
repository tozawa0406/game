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

class Dragon : public GameObject, public GUI
{
	enum class Collision : uint8
	{
		BODY = 0,
		HEAD,
		NECK1,
		NECK2,
		TAIL,
		WING_R,
		WING_L,
		MAX
	};

	enum class AttackPattern : uint8
	{
		SCREAM = 0,
		BITE,
		WING_ATTACK,
		TAKE_OFF,
		HIT,
		RUSH,

		MAX
	};
public:
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
		TAKE_OFF,
		FLY_IDLE,

		MAX
	};
	//! @def	移動速度
	static constexpr float MOVE_SPEED = 0.03f;
	//! @def	走りフラグ
	static constexpr uint IS_DASH = 0x0001;

	Dragon(void);
	~Dragon(void);

	void Init(void)	  override;
	void Uninit(void) override;
	void Update(void) override;

	void GuiUpdate(void) override;

	void Hit(int damage) override;

private:
	void CreateCollision(void);
	bool DebugInput(void);
	bool TakenDamage(void);

	void SetCollision(int arrayNum, string boneName, const Transform& offset, const MODEL& model);

	uint				flag_;
	Collider3D::OBB*	collision_[static_cast<uint8>(Collision::MAX)];

	MonsterAttack*		attack_[static_cast<int>(AttackPattern::MAX)];
	MonsterAttack*		currentAttack_;

	DragonMoveController* moveController_;

	int accumulation_;		//! 被ダメージ蓄積値

	CameraManager*	cameraManager_;
	Camera*			camera_;

	bool debugMove_;
};

#endif // _DRAGON_H_
