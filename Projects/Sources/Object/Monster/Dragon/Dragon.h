/*
 * @file		Dragon.h
 * @brief		“G‚Ì“®‚«
 * @author		ŒËàVãÄ‘¾
 * @data		2018/11/02
 */
#ifndef _DRAGON_H_
#define _DRAGON_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/Camera/CameraManager.h>
#include "../MonsterAttack.h"

class Dragon : public Object, public GUI
{
	enum class Collision : uint8
	{
		BODY = 0,
		HEAD,
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

		MAX
	};
public:
	enum class Animation : uint8
	{
		WAIT = 0,
		WALK,
		RUN,
		SCREAM,
		HIT,
		DIE,
		BITE,
		WING_ATTACK,
		TAKE_OFF,

		MAX
	};

	Dragon(void);
	~Dragon(void);

	void Init(void)	  override;
	void Uninit(void) override;
	void Update(void) override;

	void GuiUpdate(void) override;

private:
	void CreateCollision(void);
	void Move(void);
	void CreateFrontVector(void);
	void DebugInput(void);

	VECTOR3				velocity_;
	VECTOR3				front_;
	VECTOR3				right_;
	Animation			animation_;
	float				animSpeed_;
	uint				flag_;
	MeshRenderer		mesh_;
	Collider3D::OBB*	collision_[static_cast<uint8>(Collision::MAX)];

	MonsterAttack*		attack_[static_cast<int>(AttackPattern::MAX)];
	MonsterAttack*		currentAttack_;

	CameraManager*	cameraManager_;
	Camera*			camera_;


	bool debugMove_;
};

#endif // _PAID_GOODS_BOX_H_
