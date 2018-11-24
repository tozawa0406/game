#include "Slash3AttackState.h"
#include <random>

#include "Slash1AttackState.h"
#include "../DrawnState/DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	�A�j���[�V�������x
static constexpr float ANIM_SPEED = 0.6f;
//! @def	3���ڂ̃R���W�����J�n
static constexpr int COLLISION_START = 40;
//! @def	3���ڂ̃R���W�����I��
static constexpr int COLLISION_END = 65;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
Slash3AttackState::Slash3AttackState(void) : next_(false), debug_nextFrame_(false)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
Slash3AttackState::~Slash3AttackState(void)
{
}

/* @fn		Init
 * @brief	����������
 * @param	(player)	�v���C���[�ւ̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void Slash3AttackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	next_ = false;

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �ŏ��̍U�����[�V����
	meshAnim.animSpeed = ANIM_SPEED;
	meshAnim.animation = static_cast<int>(Player::Animation::Slash_3);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
}

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void Slash3AttackState::Uninit(void)
{
}

/* @fn		Update
 * @breif	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* Slash3AttackState::Update(void)
{
	if (!player_) { return nullptr; }

	Wapon* wapon = player_->GetWapon();
	if (!wapon) { return nullptr; }

	auto& meshAnim = player_->GetMeshAnimation();

	// �A�j���[�V�����̏��
	int   animMax = meshAnim.mesh.GetMaxAnimation();
	float pattern = meshAnim.mesh.GetPattern();

	// �I���O��
	if (pattern > (Quarter(animMax) * 3))
	{
		// ���̓��͂�����
		if (next_)
		{
			// ���̍U�����s��
			next_ = false;
			// �A�j���[�V�������Ō�܂ōs������ŏ��ɖ߂�
			meshAnim.animation = static_cast<int>(Player::Animation::Slash_1);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

			return new Slash1AttackState;
		}
	}

	if (pattern > COLLISION_START)
	{
		if (!wapon->IsAttack())
		{
			// ����̍U�����J�n
			wapon->AttackStart();
		}
	}
	if (pattern > COLLISION_END)
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
void Slash3AttackState::GuiUpdate(void)
{
	ImGui::Text("Slash3Attack");

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
				ImGui::Text("pattern : %.2f", player_->GetMeshAnimation().mesh.GetPattern());
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
