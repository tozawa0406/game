#include "SetupState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "PaidState/PaidMoveState.h"
#include "DrawnState/DrawnWaitState.h"
#include "DrawnState/DrawnMoveState.h"

/* @fn		�R���X�g���N�^
 * @brief	�ϐ��̏�����			*/
SetupState::SetupState(void) : isDraw_(false)
{
}

/* @fn		�f�X�g���N�^
 * @brief	...						*/
SetupState::~SetupState(void)
{
}

/* @fn		Init
 * @brief	������
 * @param	(player)	�v���C���[�̃|�C���^
 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
 * @return	�Ȃ�					*/
void SetupState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �[�����͋t�Đ�
	if (meshAnim.animation >= static_cast<int>(Player::Animation::SetupWait))
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

/* @fn		Uninit
 * @brief	�㏈��
 * @param	�Ȃ�
 * @return	�Ȃ�					*/
void SetupState::Uninit(void)
{
}

/* @fn		Update
 * @brief	�X�V����
 * @param	�Ȃ�
 * @return	���̃X�e�[�g			*/
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

			// �ړ��L�[�������Ă���ꍇ�͈ړ��X�e�[�g�փA�j���[�V������؂�ւ��Ȃ���ڍs
			if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
			{
				if (isDraw_)
				{
					meshAnim.animation = static_cast<int>(Player::Animation::Walk);
					meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
					return new PaidMoveState;
				}
				else
				{
					meshAnim.animation = static_cast<int>(Player::Animation::SetupWalk);
					meshAnim.mesh.ChangeAnimation(meshAnim.animation, 30);
					return new DrawnMoveState;
				}
			}
		}
	}

	// �L�[���͂��Ȃ��ꍇ�͑ҋ@���[�V�����ֈڍs
	if (player_->IsEndAnim())
	{
		if (isDraw_) { return new PaidWaitState;  }
		else		 { return new DrawnWaitState; }
	}

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Gui�̍X�V����
 * @param	�Ȃ�
 * @return	�Ȃ�
 * @detail	�v���C���[����Ăяo�����		*/
void SetupState::GuiUpdate(void)
{
	ImGui::Text("Setup");
}
