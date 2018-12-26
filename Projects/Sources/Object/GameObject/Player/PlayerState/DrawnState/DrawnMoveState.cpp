#include "DrawnMoveState.h"

#include "DrawnWaitState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"
#include "../AttackState/Slash1AttackState.h"

//! @def	�A�j���[�V�����̑��x
static constexpr float ANIM_SPEED = 0.55f;
//! @def	�ړ���x������
static constexpr float MOVE_DELAY = 0.75f;

DrawnMoveState::DrawnMoveState(void)
{
}

DrawnMoveState::~DrawnMoveState(void)
{
}

void DrawnMoveState::Init(Player* player, Controller* ctrl)
{
	PlayerState::Init(player, ctrl);
}

void DrawnMoveState::Uninit(void)
{
}

PlayerState* DrawnMoveState::Update(void)
{
	if (!player_ || !ctrl_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	VECTOR2 inputDir = VECTOR2(0);
	// Input
	inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
	inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
	// ���K��
	inputDir = VecNorm(inputDir);

	inputDir *= MOVE_DELAY;

	// �A�j���[�V�����؂�ւ�
	if (fabs(inputDir.x) + fabs(inputDir.y) > 0)
	{
		meshAnim.animSpeed = ANIM_SPEED;
		meshAnim.animation = static_cast<int>(Player::Animation::SetupWalk);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30);
	}
	// ���͂��Ȃ���Αҋ@����
	else
	{
		return new DrawnWaitState;;
	}

	// �ړ����x
	if (const auto& camera = player_->GetCamera())
	{
		VECTOR3 velocity = player_->GetVelocity();
		velocity += camera->GetFrontXPlane() * inputDir.y * MOVE_SPEED;
		velocity -= camera->GetRightXPlane() * inputDir.x * MOVE_SPEED;
		player_->SetVelocity(velocity);
	}

	// ����R�}���h�ŉ���X�e�[�g
	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
		{
			return new AvoidanceState;
		}
	}

	// �[���R�}���h�Ŕ[���X�e�[�g
	if (ctrl_->Trigger(Input::GAMEPAD_SQUARE, DIK_H) || ctrl_->Trigger(Input::GAMEPAD_R1, DIK_H))
	{
		return new SetupState;
	}

	// �U���R�}���h�ōU���X�e�[�g
	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		return new Slash1AttackState;
	}

	return nullptr;
}

void DrawnMoveState::GuiUpdate(void)
{
	ImGui::Text("DrawnMove");
}
