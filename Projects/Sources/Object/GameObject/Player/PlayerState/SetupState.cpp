#include "SetupState.h"
#include <random>

#include "PaidState/PaidWaitState.h"
#include "PaidState/PaidMoveState.h"
#include "DrawnState/DrawnWaitState.h"
#include "DrawnState/DrawnMoveState.h"

SetupState::SetupState(void) : isDraw_(false)
{
}

SetupState::~SetupState(void)
{
}

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
			if (isDraw_ == player_->IsDraw())
			{
				if (const auto& systems = Systems::Instance())
				{
					if (const auto& sound = systems->GetSound())
					{
						sound->Play(static_cast<int>(Resources::Sound::Camp::UNITYCHAN_DRAWN));
					}
				}

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
	if (player_->IsEndAnim())
	{
		if (isDraw_) { return new PaidWaitState; }
		else 
		{
			meshAnim.mesh.AnimEndPattern(); 
			return new DrawnWaitState; 
		}
	}

	return nullptr;
}

void SetupState::GuiUpdate(void)
{
	ImGui::Text("Setup");
}
