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

	// 納刀時は逆再生
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

	// アニメーション
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

	// 納刀抜刀中であり、アニメーションが一定以下
	if (fabs(meshAnim.mesh.GetPattern()) >= 30)
	{
		// 武器の状態切り替え
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

			// 移動キーを押している場合は移動ステートへアニメーションを切り替えながら移行
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

	// キー入力がない場合は待機モーションへ移行
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
