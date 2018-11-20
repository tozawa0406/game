#include "AttackState.h"
#include <random>

#include "DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	�A�j���[�V�������x
static constexpr float ANIM_SPEED = 0.6f;

//! @def	1���ڂ̃R���W�����J�n
static constexpr int COLLISION_SLASH1_START = 30;
//! @def	2���ڂ̃R���W�����J�n
static constexpr int COLLISION_SLASH2_START = 35;
//! @def	3���ڂ̃R���W�����J�n
static constexpr int COLLISION_SLASH3_START = 70;

//! @def	1���ڂ̃R���W�����I��
static constexpr int COLLISION_SLASH1_END = 43;
//! @def	2���ڂ̃R���W�����I��
static constexpr int COLLISION_SLASH2_END = 58;
//! @def	3���ڂ̃R���W�����I��
static constexpr int COLLISION_SLASH3_END = 110;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
AttackState::AttackState(void) : frame_(0), next_(false), debug_nextFrame_(false)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
AttackState::~AttackState(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	(player)	�v���C���[�ւ̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void AttackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	next_ = false;

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �ŏ��̍U�����[�V����
	meshAnim.animSpeed = ANIM_SPEED;
	meshAnim.animation = static_cast<int>(Player::Animation::Slash_1);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void AttackState::Uninit(void)
{
}

/* @fn		Update
 * @breif	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* AttackState::Update(void)
{
	if (!player_) { return nullptr; }

	Wapon* wapon = player_->GetWapon();
	if (!wapon) { return nullptr; }

	auto& meshAnim = player_->GetMeshAnimation();

	// �A�j���[�V�����̏��
	int animMax = meshAnim.mesh.GetMaxAnimation();
	int pattern = static_cast<int>(meshAnim.mesh.GetPattern());

	// �I���O��
	if (pattern > (Quarter(animMax) * 3))
	{
		// ���̓��͂�����
		if (next_)
		{
			// ���̍U�����s��
			frame_ = 0;
			next_ = false;
			// �A�j���[�V�������Ō�܂ōs������ŏ��ɖ߂�
			meshAnim.animation = (meshAnim.animation < static_cast<int>(Player::Animation::Slash_3)) ? static_cast<int>(meshAnim.animation) + 1 : static_cast<int>(Player::Animation::Slash_1);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
		}
	}

	frame_++;
	int start[] = { COLLISION_SLASH1_START, COLLISION_SLASH2_START, COLLISION_SLASH3_START };
	int end[] = { COLLISION_SLASH1_END, COLLISION_SLASH2_END, COLLISION_SLASH3_END };

	int num = ((sizeof(start) / sizeof(start[0])) - 1) - (static_cast<int>(Player::Animation::Slash_3) - meshAnim.animation);

	if (frame_ > start[num])
	{
		if (!wapon->IsAttack())
		{
			// ����̍U�����J�n
			wapon->AttackStart();
		}
	}
	if (frame_ > end[num])
	{
		// ����̍U�����I��
		wapon->AttackEnd();
	}

	// �A�j���[�V�����̏I��
	if (player_->IsEndAnim())
	{
		// �����ҋ@��Ԃɖ߂�
		meshAnim.animation = static_cast<int>(Player::Animation::SetupWait);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);

		return new DrawnWaitState;
	}

	// �U���R�}���h�Ŏ��̍U��
	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U)) { next_ = true; }

	// ����R�}���h�ŉ���X�e�[�g
	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
		{
			return new AvoidanceState;
		}
	}

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ăяo�����			*/
void AttackState::GuiUpdate(void)
{
	ImGui::Text("Attack");

	if (const auto& systems = Systems::Instance())
	{
		if (const auto& debug = systems->GetDebug())
		{
			if (debug->GetDebugPause())
			{
				if (ImGui::Button("Next Frame"))
				{
					debug_nextFrame_ = true;
					debug->SetDebugPause(false);
				}
				ImGui::SameLine();
				ImGui::Text("frame : %d", frame_);
			}
			else
			{
				if (debug_nextFrame_)
				{
					debug->SetDebugPause(true);
				}
				debug_nextFrame_ = false;
			}
		}
	}
}
