#include "Slash3AttackState.h"
#include <random>

#include "Slash1AttackState.h"
#include "../DrawnState/DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	コリジョン開始
static constexpr int COLLISION_START = 40;
//! @def	コリジョン終了
static constexpr int COLLISION_END = 65;
//! @def	移動距離
static constexpr float MOVE = 0.1f;
//! @def	エフェクトの回転角度
static constexpr float EFFECT_ROTATION = 0;
//! @def	移動開始
static constexpr int MOVE_START = 37;
//! @def	移動終了
static constexpr int MOVE_END = 42;

Slash3AttackState::Slash3AttackState(void)
{
}

Slash3AttackState::~Slash3AttackState(void)
{
}

void Slash3AttackState::Init(Player* player, Controller* ctrl)
{	
	if (!player) { return; }
	
	animSpeed_		= ANIM_SPEED;
	animation_		= Player::Animation::Slash_3;
	changeFrame_	= ANIMATION_CHANGE_FRAME30;
	collisionStart_ = COLLISION_START;
	collisionEnd_	= COLLISION_END;
	effectRot_		= EFFECT_ROTATION;
	moveStart_		= MOVE_START;
	moveEnd_		= MOVE_END;
	move_			= MOVE;
	voice_			= static_cast<int>(Resources::Sound::Camp::UNITYCHAN_ATTACK03);

	AttackBaseState::Init(player, ctrl);

	if (auto* wapon = player_->GetWapon())
	{
		wapon->SetOffensivePower(100);
	}
}

void Slash3AttackState::Uninit(void)
{
	AttackBaseState::Uninit();
}

PlayerState* Slash3AttackState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	if (auto temp = AttackBaseState::Update())
	{
		return temp;
	}

	float pattern = meshAnim.mesh.GetPattern();
	// アニメーションの情報
	int animMax = meshAnim.mesh.GetMaxAnimation();
	// 終了前に
	if (pattern > (Quarter(animMax) * 3.0f))
	{
		// 回避コマンドで回避ステート
		if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
		{
			if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
			{
				return new AvoidanceState;
			}
		}

		if (next_)
		{
			// 次の攻撃を行う
			next_ = false;
			// アニメーションが最後まで行ったら最初に戻る
			meshAnim.animation = static_cast<int>(Player::Animation::Slash_1);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

			return new Slash1AttackState;
		}
	}

	return nullptr;
}

void Slash3AttackState::GuiUpdate(void)
{
	ImGui::Text("Slash3Attack");

	AttackBaseState::GuiUpdate();
}
