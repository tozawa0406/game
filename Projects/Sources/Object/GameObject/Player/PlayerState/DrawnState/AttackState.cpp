#include "AttackState.h"
#include <random>

#include "DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	�A�j���[�V�������x
static constexpr float ANIM_SPEED = 0.6f;

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
AttackState::AttackState(void) : next_(false)
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

	// ����̍U�����J�n
	if (const auto& wapon = player->GetWapon())
	{
		wapon->AttackStart();
	}
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
			if (const auto& wapon = player_->GetWapon())
			{
				wapon->AttackStart();
			}
			next_ = false;
			// �A�j���[�V�������Ō�܂ōs������ŏ��ɖ߂�
			meshAnim.animation = (meshAnim.animation < static_cast<int>(Player::Animation::Slash_3)) ? static_cast<int>(meshAnim.animation) + 1 : static_cast<int>(Player::Animation::Slash_1);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);
		}
	}

	// �A�j���[�V�����̏I��
	if (player_->IsEndAnim())
	{
		if (const auto& wapon = player_->GetWapon())
		{
			wapon->AttackEnd();
		}
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
		return new AvoidanceState;
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
}
