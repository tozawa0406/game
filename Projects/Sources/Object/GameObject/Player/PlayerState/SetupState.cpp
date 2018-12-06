#include "SetupState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "PaidState/PaidMoveState.h"
#include "DrawnState/DrawnWaitState.h"
#include "DrawnState/DrawnMoveState.h"

/* @brief	�R���X�g���N�^			*/
SetupState::SetupState(void) : isDraw_(false)
{
}

/* @brief	�f�X�g���N�^			*/
SetupState::~SetupState(void)
{
}

/* @brief	����������
 * @param	(player)	�v���C���[�̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void SetupState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �[�����͋t�Đ�
	if (player->IsDraw())
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
	meshAnim.animation = static_cast<int>(Player::Animation::Setup);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
}

/* @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void SetupState::Uninit(void)
{
}

/* @brief	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
PlayerState* SetupState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();
	int max = meshAnim.mesh.GetMaxAnimation();
	int pattern = static_cast<int>(meshAnim.mesh.GetPattern());

	// �[���������ł���A�A�j���[�V���������ȉ�
	if (fabs(meshAnim.mesh.GetPattern()) >= 30)
	{
		// ����̏�Ԑ؂�ւ�
		if (const auto& wapon = player_->GetWapon())
		{
			if (isDraw_ == player_->IsDraw())
			{
				wapon->Setup(isDraw_);
				player_->SetDraw(!player_->IsDraw());
			}

			// �ړ��L�[�������Ă���ꍇ�͈ړ��X�e�[�g�փA�j���[�V������؂�ւ��Ȃ���ڍs
			if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
			{
				if (isDraw_)
				{
					meshAnim.animation = static_cast<int>(Player::Animation::Walk);
					meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30, true);
					return new PaidMoveState;
				}
				else
				{
					meshAnim.animation = static_cast<int>(Player::Animation::SetupWalk);
					meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30, true);
					return new DrawnMoveState;
				}
			}
		}
	}

	// �L�[���͂��Ȃ��ꍇ�͑ҋ@���[�V�����ֈڍs
	if (isDraw_)
	{
		if (player_->IsEndAnim()) { return new PaidWaitState; }
	}
	else
	{
		if (pattern >= max - 1) { return new DrawnWaitState; }
	}

	return nullptr;
}

/* @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ă΂��	*/
void SetupState::GuiUpdate(void)
{
	ImGui::Text("Setup");
}
