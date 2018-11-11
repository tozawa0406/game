#include "AvoidanceState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "PaidState/PaidMoveState.h"
#include "DrawnState/DrawnWaitState.h"
#include "DrawnState/DrawnMoveState.h"

//! @def	�ړ����x
static constexpr float MOVE_SPEED = 0.06f;
//! @def	��𑬓x
static constexpr float AVOIDANCE_SPEED = 2.75f;
//! @def	�A�j���[�V�����ύX���x
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;
//! @def	�A�j���[�V�����̑��x
static constexpr float ANIMATION_DEFAULT = 0.75f;
//! @def	����A�j���[�V�����̏I���t���[��
static constexpr int END_AVOIDANCE_ANIMATION = 30;

AvoidanceState::AvoidanceState(void) : dir_(VECTOR3(0)), isDraw_(false)
{
}

AvoidanceState::~AvoidanceState(void)
{
}

void AvoidanceState::Init(PlayerHunter* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	PlayerMove::Animation temp = static_cast<PlayerMove::Animation>(meshAnim.animation);
	isDraw_ = (temp == PlayerMove::Animation::SetupWait || temp == PlayerMove::Animation::SetupWalk) ? true: false;

	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = ANIMATION_DEFAULT;
	meshAnim.animation = static_cast<int>(PlayerMove::Animation::Roll);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);

	// ���͕����ɉ���A���͂��Ȃ��Ƃ��͑O�ɉ��
	if (ctrl_)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// ���K��
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

void AvoidanceState::Uninit(void)
{
}

PlayerState* AvoidanceState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	if (meshAnim.mesh.GetPattern() >= END_AVOIDANCE_ANIMATION)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// ���K��
		inputDir = VecNorm(inputDir);

		VECTOR3 velocity = player_->GetVelocity();
		velocity *= 0.5f;
		player_->SetVelocity(velocity);


		if (inputDir != 0)
		{
			if (isDraw_) { return new DrawnMoveState; }
			else		 { return new PaidMoveState;  }
		}
		else
		{
			// �[����ԂƔ�����ԂŃA�j���[�V�����̐؂�ւ�
			meshAnim.animation = static_cast<int>((isDraw_) ? PlayerMove::Animation::SetupWait : PlayerMove::Animation::Wait);
			// �A�j���[�V�����̕ύX
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15, false);

			if (isDraw_) { return new DrawnWaitState; }
			else		 { return new PaidWaitState;  }
		}
	}


	// �ړ����x
	VECTOR3 velocity = player_->GetVelocity();
	velocity += dir_ * MOVE_SPEED * AVOIDANCE_SPEED;
	player_->SetVelocity(velocity);

	return nullptr;
}
