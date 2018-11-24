#include "EmergencyAvoidanceState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "PaidState/PaidMoveState.h"
#include "DrawnState/DrawnWaitState.h"
#include "DrawnState/DrawnMoveState.h"

//! @def	��𑬓x
static constexpr float AVOIDANCE_SPEED = 2.75f;
//! @def	����A�j���[�V�������x
static constexpr float AVOIDANCE_ANIM_SPEED = 0.5f;
//! @def	�����オ��A�j���[�V�����̑��x
static constexpr float STANDUP_ANIM_SPEED = 3;
//! @def	�ړ�����t���[��
static constexpr int   MOVE_FRAME = 75;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
EmergencyAvoidanceState::EmergencyAvoidanceState(void) : dir_(VECTOR3(0)), cnt_(0)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
EmergencyAvoidanceState::~EmergencyAvoidanceState(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	(player)	�v���C���[�ւ̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void EmergencyAvoidanceState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �U�����f
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackEnd();
	}
	meshAnim.animSpeed = AVOIDANCE_ANIM_SPEED;
	meshAnim.animation = static_cast<int>(Player::Animation::Dive);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

	// ���͕����ɉ���A���͂��Ȃ��Ƃ��͑O�ɉ��
	if (ctrl_)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// ���K��
		inputDir = VecNorm(inputDir);

		// ��������
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

	if (auto collider = player->GetCollider())
	{
		collider->SetEnable(false);
	}

	player->SetStamina(player->GetStamina() - DEC_STAMINA);
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void EmergencyAvoidanceState::Uninit(void)
{
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* EmergencyAvoidanceState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	player_->SetStamina(player_->GetStamina() - 0.1f);

	cnt_++;
	//�I����
	if (player_->IsEndAnim())
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// ���K��
		inputDir = VecNorm(inputDir);

		VECTOR3 velocity = Half(player_->GetVelocity());
		player_->SetVelocity(velocity);

		if (auto collider = player_->GetCollider())
		{
			collider->SetEnable(true);
		}
		meshAnim.mesh.AnimEndPattern();
		// ���͂�����Έړ��X�e�[�g
		if (inputDir != 0)
		{
			meshAnim.animation = static_cast<int>(Player::Animation::Walk);
			// �A�j���[�V�����̕ύX
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);
			return new PaidMoveState;
		}
		else
		{
			meshAnim.animation = static_cast<int>(Player::Animation::Wait);
			// �A�j���[�V�����̕ύX
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);
			return new PaidWaitState;
		}
	}

	if (cnt_ < MOVE_FRAME)
	{
		// �ړ����x
		VECTOR3 velocity = player_->GetVelocity();
		velocity += dir_ * MOVE_SPEED * AVOIDANCE_SPEED;
		player_->SetVelocity(velocity);
	}
	else
	{
		meshAnim.animSpeed = STANDUP_ANIM_SPEED;
	}

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ăяo�����		*/
void EmergencyAvoidanceState::GuiUpdate(void)
{
	ImGui::Text("EmergencyAvoidance");
	ImGui::Text("cnt : %d", cnt_);
}
