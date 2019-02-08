#include "DragonMoveController.h"
#include "Dragon.h"
#include <random>

//! @def	走る範囲
static constexpr int DASH_RANGE = 75;
//! @def	攻撃のスパン
static constexpr int ATTACK_RANGE = 90;

DragonMoveController::DragonMoveController(void) : GUI(Systems::Instance(), nullptr, "DragonController")
	, parent_(nullptr)
	, target_(nullptr)
	, isDash_(false)
	, cnt_(0)
	, collider_(nullptr)
{
}

DragonMoveController::~DragonMoveController(void)
{
}

void DragonMoveController::Init(Dragon* parent)
{
	parent_ = parent;
	collider_ = new Collider3D::Sphere(parent);
	if (collider_)
	{
		collider_->SetSize(200);
	}
}

void DragonMoveController::Uninit(void)
{
	DeletePtr(collider_);
}

void DragonMoveController::Action(int& act, uint& flag)
{
	// 索敵範囲内にプレイヤーが入った時のみバインドボイス
	if (SearchTarget()) { act = 0; return; }
	if (!parent_ || !target_) { act = -1; return; }
	const int range = 19;
	if (act < 0) { return; }

	// 方決定
	dir_ = target_->GetTransform().position - parent_->GetTransform().position;
	dir_.y = 0;

	// 走り判定
	float inputDash = 1;
	BitSub(flag, Dragon::IS_DASH);

	float lenghtSq = VecLengthSq(dir_);

	if (lenghtSq > DASH_RANGE * DASH_RANGE)
	{
		isDash_ = true;
		inputDash = 10;
		BitAdd(flag, Dragon::IS_DASH);
	}
	else
	{
		// 1回走ったなら中途半端に終わるな
		if (isDash_)
		{
			inputDash = 10;
			BitAdd(flag, Dragon::IS_DASH);

			// 1ループしたら終了してよい
			if (parent_->IsEndAnim()) { isDash_ = false; }
		}
	}

	dir_ = (lenghtSq < range * range) ? 0 : VecNorm(dir_);

	VECTOR3 velocity = parent_->GetVelocity();
	velocity += VECTOR3(0, 0, 1) * dir_.z * inputDash * Dragon::MOVE_SPEED;
	velocity += VECTOR3(1, 0, 0) * dir_.x * inputDash * Dragon::MOVE_SPEED;
	parent_->SetVelocity(velocity);

	auto& meshAnim = parent_->GetMeshAnimation();

	Dragon::Animation tempAnim = static_cast<Dragon::Animation>(meshAnim.animation);
	if (dir_ != 0)
	{
		act = -1;
		if (tempAnim == Dragon::Animation::WAIT1 || (tempAnim == Dragon::Animation::WALK || tempAnim == Dragon::Animation::RUN))
		{
			meshAnim.animSpeed = 0.5f;
			meshAnim.animation = static_cast<int>(Dragon::Animation::WALK);
			if (BitCheck(flag, Dragon::IS_DASH))
			{
				meshAnim.animSpeed = 0.75f;
				meshAnim.animation = static_cast<int>(Dragon::Animation::RUN);
			}
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
		}
	}
	else
	{
		if (tempAnim == Dragon::Animation::WALK || tempAnim == Dragon::Animation::RUN)
		{
			meshAnim.mesh.ChangeAnimation(static_cast<int>(Dragon::Animation::WAIT1), 30);
			meshAnim.animSpeed = 0.75f;
		}
		if (act == 0)
		{
			cnt_++;
			act = -1;
			if (cnt_ > ATTACK_RANGE)
			{
				cnt_ = 0;
				std::random_device randDev;
				act = (randDev() % 4) + 1;
			}
		}
	}
}

bool DragonMoveController::SearchTarget(void)
{
	if (!collider_ || !collider_->IsEnable()) { return false; }

	auto objects = collider_->Hit();
	for (auto o : objects)
	{
		if (o->GetTag() == ObjectTag::PLAYER)
		{
			target_ = static_cast<GameObject*>(o);
			collider_->SetEnable(false);
			return true;
		}
	}
	return false;
}

void DragonMoveController::GuiUpdate(void)
{
	ImGui::Text("dir : %.2f,%.2f, %.2f", dir_.x, dir_.y, dir_.z);
}
