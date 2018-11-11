#include "SetupState.h"
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

SetupState::SetupState(void) : isDraw_(false)
{
}

SetupState::~SetupState(void)
{
}

void SetupState::Init(PlayerHunter* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �[�����͋t�Đ�
	PlayerMove::Animation temp = static_cast<PlayerMove::Animation>(meshAnim.animation);
	if (temp == PlayerMove::Animation::SetupWait || temp == PlayerMove::Animation::SetupWalk) 
	{
		isDraw_ = true;
		meshAnim.animSpeed = -0.75f; 
	}
	else
	{
		isDraw_ = false;
		meshAnim.animSpeed = 0.75f;
	}

	// �A�j���[�V����
	meshAnim.animation = static_cast<int>(PlayerMove::Animation::Setup);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
}

void SetupState::Uninit(void)
{
}

PlayerState* SetupState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	// �[���������ł���A�A�j���[�V���������ȉ�
	if (fabs(meshAnim.mesh.GetPattern()) >= 30)
	{
		// ����̏�Ԑ؂�ւ�
		if (const auto& wapon = player_->GetWapon())
		{
			wapon->Setup(isDraw_);
			if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
			{
				if (isDraw_)
				{
					meshAnim.animation = static_cast<int>(PlayerMove::Animation::Walk);
					meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
					return new PaidMoveState;
				}
				else
				{
					meshAnim.animation = static_cast<int>(PlayerMove::Animation::SetupWalk);
					meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
					return new DrawnMoveState;
				}
			}
		}
	}

	if (player_->IsEndAnim())
	{
		if (isDraw_) { return new PaidWaitState;  }
		else		 { return new DrawnWaitState; }
	}

	return nullptr;
}
