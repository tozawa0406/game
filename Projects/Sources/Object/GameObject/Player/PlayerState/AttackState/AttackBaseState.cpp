#include "AttackBaseState.h"
#include "../DrawnState/DrawnWaitState.h"

//! @def	アニメーション速度
static constexpr float ANIM_SPEED = 0.6f;
//! @def	コリジョン開始
static constexpr int COLLISION_START = 18;
//! @def	コリジョン終了
static constexpr int COLLISION_END = 26;
//! @def	移動距離
static constexpr float MOVE = 0.1f;
//! @def	移動開始
static constexpr int MOVE_START = 10;
//! @def	移動終了
static constexpr int MOVE_END = 15;

/* @brief	コンストラクタ			*/
AttackBaseState::AttackBaseState(void) : dir_(VECTOR3(0))
	, next_(false)
	, debug_nextFrame_(false)
	, animSpeed_(0)
	, animation_(Player::Animation::Wait)
	, changeFrame_(0)
	, collisionStart_(0)
	, collisionEnd_(0)
	, moveStart_(0)
	, moveEnd_(0)
	, move_(0)
{
}

/* @brief	デストラクタ			*/
AttackBaseState::~AttackBaseState(void)
{
}

/* @brief	初期化処理
 * @param	(player)	プレイヤーへのポインタ
 * @param	(ctrl)		コントローラへのポインタ
 * @return	なし					*/
void AttackBaseState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	next_ = false;

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// 最初の攻撃モーション
	meshAnim.animSpeed = animSpeed_;
	meshAnim.animation = static_cast<int>(animation_);
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, changeFrame_);

	// 入力方向に回避、入力がないときは前に回避
	if (ctrl_)
	{
		VECTOR2 inputDir;
		// Input
		inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
		inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
		// 正規化
		inputDir = VecNorm(inputDir);

		// 方向決め
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
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void AttackBaseState::Uninit(void)
{
}

/* @breif	更新処理
 * @param	なし
 * @return	次のステート			*/
PlayerState* AttackBaseState::Update(void)
{
	if (!player_) { return nullptr; }

	Wapon* wapon = player_->GetWapon();
	if (!wapon) { return nullptr; }

	auto& meshAnim	= player_->GetMeshAnimation();
	int max			= meshAnim.mesh.GetMaxAnimation();

	// アニメーションの情報
	float pattern = meshAnim.mesh.GetPattern();

	if (pattern > collisionStart_)
	{
		if (!wapon->IsAttack())
		{
			// 武器の攻撃を開始
			wapon->AttackStart();
		}
	}
	if (pattern > collisionEnd_)
	{
		// 武器の攻撃を終了
		wapon->AttackEnd();
	}

	if(moveStart_ < pattern && pattern < moveEnd_)
	{
		auto v = player_->GetVelocity();
		if (const auto& camera = player_->GetCamera())
		{
			v += dir_ * move_;
		}
		player_->SetVelocity(v);
	}

	// アニメーションの終了
	if (pattern >= max - 1)
	{
		// 抜刀待機状態に戻る
		meshAnim.animation = static_cast<int>(Player::Animation::SetupWait);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15, true);

		return new DrawnWaitState;
	}

	// 攻撃コマンドで次の攻撃
	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U)) { next_ = true; }

	return nullptr;
}

/* @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	プレイヤーから呼ばれる	*/
void AttackBaseState::GuiUpdate(void)
{
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
