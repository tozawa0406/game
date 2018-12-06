#include "PaidMoveState.h"

#include "PaidWaitState.h"
#include "../AvoidanceState.h"
#include "../EmergencyAvoidanceState.h"
#include "../AttackState/SetupAttackState.h"

//! @def	����X�^�~�i
static constexpr float DEC_STAMINA = 0.1f;

/* @brief	�R���X�g���N�^			*/
PaidMoveState::PaidMoveState(void)
{
}

/* @brief	�f�X�g���N�^			*/
PaidMoveState::~PaidMoveState(void)
{
}

/* @brief	����������
 * @param	(player)	�v���C���[�ւ̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void PaidMoveState::Init(Player* player, Controller* ctrl)
{
	PlayerState::Init(player, ctrl);
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void PaidMoveState::Uninit(void)
{
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* PaidMoveState::Update(void)
{
	if (!player_ || !ctrl_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	// ����
	VECTOR2 inputDir;
	// Input
	inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
	inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
	// ���K��
	inputDir = VecNorm(inputDir);

	bool dash = (ctrl_->Trigger(Input::GAMEPAD_L3, DIK_LSHIFT)) ? true : false;

	float inputDash = 1.0f;
	if (ctrl_->Press(Input::GAMEPAD_R1, DIK_LSHIFT) || dash)
	{
		float stamina = player_->GetStamina();
		if (stamina > 0)
		{
			inputDash = 2.5f;
			player_->SetStamina(stamina - (Player::ADD_STAMINA + DEC_STAMINA));
		}
	}

	inputDir *= inputDash;

	// �A�j���[�V�����؂�ւ�
	if (fabs(inputDir.x) + fabs(inputDir.y) > 0)
	{
		meshAnim.animSpeed = 0.55f;
		meshAnim.animation = static_cast<int>((inputDash <= 1) ? Player::Animation::Walk : Player::Animation::Run);
		int cnt = (meshAnim.animation == static_cast<int>(Player::Animation::Run)) ? 15 : ANIMATION_CHANGE_FRAME30;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, cnt);
	}
	// ���͂��Ȃ��Ƒҋ@���[�V������
	else
	{
		return new PaidWaitState;
	}

	// �ړ����x
	if (const auto& camera = player_->GetCamera())
	{
		VECTOR3 velocity = player_->GetVelocity();
		velocity += camera->GetFrontXPlane() * inputDir.y * MOVE_SPEED;
		velocity -= camera->GetRightXPlane() * inputDir.x * MOVE_SPEED;
		player_->SetVelocity(velocity);
	}

	// ����R�}���h�ŉ���X�e�[�g��
	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
		{
			if (inputDash == 2.5f)
			{
				return new EmergencyAvoidanceState;
			}
			else
			{
				return new AvoidanceState;
			}
		}
	}

	// �����R�}���h�Ŕ����X�e�[�g��
	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		return new SetupAttackState;
	}
	
	return nullptr;
}

/* @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ă΂��	*/
void PaidMoveState::GuiUpdate(void)
{
	ImGui::Text("PaidMove");
}
