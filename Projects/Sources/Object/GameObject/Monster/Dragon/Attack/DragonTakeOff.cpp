#include "DragonTakeOff.h"
#include "../Dragon.h"

/* @brief	コンストラクタ			*/
DragonTakeOff::DragonTakeOff(void) : fly_(false)
	, debug_velocity_(0)
{
}

/* @brief	デストラクタ			*/
DragonTakeOff::~DragonTakeOff(void)
{
}

/* @brief	初期化処理
 * @param	(object)	このクラスでは使わない
 * @return	なし					*/
void DragonTakeOff::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void DragonTakeOff::Uninit(void)
{
}

/* @brief	実行する準備
 * @param	なし
 * @return	なし
 * @detail	この攻撃特有のアニメーション、速度を設定する		*/
void DragonTakeOff::SetMove(void)
{
	// 既に使用中なら重複防止
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	// 速度の設定
	meshAnim.animSpeed = fly_ ? -0.75f : 0.75f;

	// アニメーションの設定
	meshAnim.animation = static_cast<int>(Dragon::Animation::TAKE_OFF);

	// 実際のアニメーションの切り替え
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
}

/* @brief	更新処理
 * @param	なし
 * @return	攻撃が終了したらtrue
 * @detail	姿勢変更はなし、移動しないよう速度は0に固定
			一定時間経過後、アニメーションの速度を上げる、アニメーションが終了したら元に戻って終了		*/
bool DragonTakeOff::Update(void)
{
	if (!monster_) { return true; }

	auto velocity = monster_->GetVelocity();
	int sign = (fly_) ? -1 : 1;
	velocity.x = 0;
	velocity.z = 0;
	velocity.y += 0.1f * sign;
	monster_->SetVelocity(velocity);

	debug_velocity_ = velocity.y;

	// 演出用
	frame_++;

	// アニメーション終了
	if (monster_->IsEndAnim())
	{
		auto& meshAnim = monster_->GetMeshAnimation();
		// 元に戻す
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>((fly_) ? Dragon::Animation::WAIT1 : Dragon::Animation::FLY_IDLE);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 5, true);
		return true;
	}
	return false;
}

/* @brief	モーションの終了処理
 * @param	なし
 * @return	なし			*/
void DragonTakeOff::EndMove(void)
{
}

/* @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	攻撃元オブジェクトから呼ばれる		*/
void DragonTakeOff::GuiUpdate(void)
{
	ImGui::Text("frame : %d", frame_);
	ImGui::Text("velocity : %.2f", debug_velocity_);
	ImGui::Text("y : %.2f", monster_->GetTransform().position.y);
}
