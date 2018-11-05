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

class Dragon : public Object
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

	enum class Animation : uint8
	{
		WAIT = 0,
		WALK,
		RUN,
		SCREAM,
		HIT,
		DIE,

		MAX
	};

public:
	Dragon(void);
	~Dragon(void);

	void Init(void)	  override;
	void Uninit(void) override;
	void Update(void) override;

private:
	uint8		 animNum_;
	MeshRenderer mesh_;
	Collider3D::OBB* collision_[static_cast<uint8>(Collision::MAX)];
};

#endif // _PAID_GOODS_BOX_H_
