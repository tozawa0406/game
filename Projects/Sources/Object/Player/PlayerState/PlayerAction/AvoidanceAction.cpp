#include "AvoidanceAction.h"
#include <random>

#include "PaidAction/PaidWaitAction.h"
#include "PaidAction/PaidMoveAction.h"

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
		// ���K��
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
			// �[����ԂƔ�����ԂŃA�j���[�V�����̐؂�ւ�
			meshAnim.animation = static_cast<int>(PlayerMove::Animation::Wait);
			// �A�j���[�V�����̕ύX
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15, false);

			return new PaidWaitAction;
		}
	}


	// �ړ����x
	VECTOR3 velocity = player_->GetVelocity();
	velocity += dir_ * MOVE_SPEED * AVOIDANCE_SPEED;
	player_->SetVelocity(velocity);

	return nullptr;
}
