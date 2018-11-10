#include "DrawnMoveAction.h"

//! @def	�ړ����x
static constexpr float MOVE_SPEED = 0.06f;
//! @def	�A�j���[�V�����ύX���x
static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;

DrawnMoveAction::DrawnMoveAction(void)
{
}

DrawnMoveAction::~DrawnMoveAction(void)
{
}

void DrawnMoveAction::Init(PlayerHunter* player, Controller* ctrl)
{
	PlayerAction::Init(player, ctrl);

	inputDir_  = VECTOR2(0);
}

void DrawnMoveAction::Uninit(void)
{
}

PlayerAction* DrawnMoveAction::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	if (ctrl_)
	{
		// Input
		inputDir_.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir_.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// ���K��
		inputDir_ = VecNorm(inputDir_);
	}

	inputDir_ *= 0.75f;

	// �A�j���[�V�����؂�ւ�
	if (fabs(inputDir_.x) + fabs(inputDir_.y) > 0)
	{
		meshAnim.animSpeed = 0.55f;
		meshAnim.animation = static_cast<int>(PlayerMove::Animation::SetupWalk);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30);
	}
	else
	{
		// ���̃X�e�[�g
		return nullptr;
	}

	// �ړ����x
	if (const auto& camera = player_->GetCamera())
	{
		VECTOR3 velocity = player_->GetVelocity();
		velocity += camera->GetFrontXPlane() * inputDir_.y * MOVE_SPEED;
		velocity -= camera->GetRightXPlane() * inputDir_.x * MOVE_SPEED;
		player_->SetVelocity(velocity);
	}

	return nullptr;
}
