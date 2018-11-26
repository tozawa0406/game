#include "Slash2AttackState.h"
#include <random>

#include "Slash3AttackState.h"
#include "../DrawnState/DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	アニメーション速度
static constexpr float ANIM_SPEED = 0.6f;
//! @def	コリジョン開始
static constexpr int COLLISION_START = 21;
//! @def	コリジョン終了
static constexpr int COLLISION_END = 35;
//! @def	移動距離
static constexpr float MOVE = 0.1f;
//! @def	移動開始
static constexpr int MOVE_START = 10;
//! @def	移動終了
static constexpr int MOVE_END = 15;


/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
Slash2AttackState::Slash2AttackState(void)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
Slash2AttackState::~Slash2AttackState(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	(player)	プレイヤーへのポインタ
 * @param	(ctrl)		コントローラへのポインタ
 * @return	なし					*/
void Slash2AttackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }
	
	animSpeed_		= ANIM_SPEED;
	animation_		= Player::Animation::Slash_2;
	changeFrame_	= ANIMATION_CHANGE_FRAME30;
	collisionStart_ = COLLISION_START;
	collisionEnd_	= COLLISION_END;
	moveStart_		= MOVE_START;
	moveEnd_		= MOVE_END;
	move_			= MOVE;

	AttackBaseState::Init(player, ctrl);
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void Slash2AttackState::Uninit(void)
{
}

/* @fn		Update
 * @breif	更新処理
 * @param	なし
 * @return	次のステート			*/
PlayerState* Slash2AttackState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	if (auto wapon = player_->GetWapon())
	{
		int inv = (dir_.z >= 0) ? 1 : -1;
		wapon->SetRotation(-1.57f * inv);
	}

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
			meshAnim.animation = static_cast<int>(Player::Animation::Slash_3);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

			return new Slash3AttackState;
		}
	}

	return nullptr;
}

/* @fn		GuiUpdate
 * @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	プレイヤーから呼び出される			*/
void Slash2AttackState::GuiUpdate(void)
{
	ImGui::Text("Slash2Attack");

	AttackBaseState::GuiUpdate();
}
