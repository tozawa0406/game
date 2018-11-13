/*
 * @file		Dragon.h
 * @brief		“G‚Ì“®‚«
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/02
 */
#ifndef _DRAGON_H_
#define _DRAGON_H_

#include <FrameWork/Systems/Camera/CameraManager.h>
#include "../../GameObject.h"
#include "../MonsterAttack.h"

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

	Dragon(void);
	~Dragon(void);

	void Init(void)	  override;
	void Uninit(void) override;
	void Update(void) override;

	void GuiUpdate(void) override;

	void Hit(void) override {}

private:
	void CreateCollision(void);
	void DebugInput(void);

	uint				flag_;
	Collider3D::OBB*	collision_[static_cast<uint8>(Collision::MAX)];

	MonsterAttack*		attack_[static_cast<int>(AttackPattern::MAX)];
	MonsterAttack*		currentAttack_;

	CameraManager*	cameraManager_;
	Camera*			camera_;

	bool debugMove_;
};

#endif // _DRAGON_H_
