#include "AvoidanceAction.h"
#include <random>

#include "PaidAction/PaidWaitAction.h"
#include "PaidAction/PaidMoveAction.h"

//! @def	移動速度
static constexpr float MOVE_SPEED = 0.06f;
//! @def	回避速度
static constexpr float AVOIDANCE_SPEED = 2.75f;
//! @def	アニメーション変更速度
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;
//! @def	アニメーションの速度
static constexpr float ANIMATION_DEFAULT = 0.75f;
//! @def	回避アニメーションの終了フレーム
static constexpr int END_AVOIDANCE_ANIMATION = 30;

AvoidanceAction::AvoidanceAction(void) : dir_(VECTOR3(0))
{
}

AvoidanceAction::~AvoidanceAction(void)
{
}

void AvoidanceAction::Init(PlayerHunter* player, Controller* ctrl)
{
	if (!player_) { return; }

	PlayerAction::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = ANIMATION_DEFAULT;
	meshAnim.animation = static_cast<int>(PlayerMove::Animation::Roll);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);

	// 入力方向に回避、入力がないときは前に回避
	if (ctrl_)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// 正規化
		inputDir = VecNorm(inputDir);

		if (inputDir == 0)
		{
			dir_ -= player->GetFront();
		}
		else
		{
			if (const auto& camera_ = player->GetCamera())
			{
				dir_ = VECTOR3(0);
				dir_ += camera_->GetFrontXPlane() * inputDir.y;
				dir_ -= camera_->GetRightXPlane() * inputDir.x;
				dir_ = VecNorm(dir_);
			}
		}
	}
}

void AvoidanceAction::Uninit(void)
{
}

PlayerAction* AvoidanceAction::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	if (meshAnim.mesh.GetPattern() >= END_AVOIDANCE_ANIMATION)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// 正規化
		inputDir = VecNorm(inputDir);

		VECTOR3 velocity = player_->GetVelocity();
		velocity *= 0.5f;
		player_->SetVelocity(velocity);


		if (inputDir != 0)
		{
			return new PaidMoveAction;
		}
		else
		{
			// 納刀状態と抜刀状態でアニメーションの切り替え
			meshAnim.animation = static_cast<int>(PlayerMove::Animation::Wait);
			// アニメーションの変更
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15, false);

			return new PaidWaitAction;
		}
	}


	// 移動速度
	VECTOR3 velocity = player_->GetVelocity();
	velocity += dir_ * MOVE_SPEED * AVOIDANCE_SPEED;
	player_->SetVelocity(velocity);

	return nullptr;
}
