#include "DragonRush.h"
#include "../Dragon.h"

/* @brief	コンストラクタ			*/
DragonRush::DragonRush(void) : cnt_(0)
{
}

/* @brief	デストラクタ			*/
DragonRush::~DragonRush(void)
{
}

/* @brief	初期化処理
 * @param	(object)	当たり判定の親クラスに登録
 * @return	なし					*/
void DragonRush::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void DragonRush::Uninit(void)
{
}

/* @brief	実行する準備
 * @param	なし
 * @return	なし
 * @detail	この攻撃特有のアニメーション、速度を設定する		*/
void DragonRush::SetMove(void)
{
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	meshAnim.animSpeed = 0.5f;
	meshAnim.animation   = static_cast<int>(Dragon::Animation::WAIT2);

	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
	cnt_ = 0;
}

/* @brief	更新処理
 * @param	なし
 * @return	攻撃が終了したらtrue
 * @detail	姿勢変更はなし、移動しないよう速度は0に固定
			一定時間経過後、アニメーションの速度を上げる、アニメーションが終了したら元に戻って終了		*/
bool DragonRush::Update(void)
{
	if (!monster_) { return true; }
	auto& meshAnim = monster_->GetMeshAnimation();

	cnt_++;
	if (cnt_ > 30)
	{
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::RUN);

		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);

		VECTOR3 front = monster_->GetFront();
		monster_->SetVelocity(-front);
	}

	if (cnt_ > 148)
	{
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15, true);
		return true;
	}
	return false;
}

/* @brief	モーションの終了処理
 * @param	なし
 * @return	なし			*/
void DragonRush::EndMove(void)
{
}

/* @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	攻撃元オブジェクトから呼ばれる		*/
void DragonRush::GuiUpdate(void)
{
	MonsterAttack::GuiUpdate();

	ImGui::Text("cnt : %d", cnt_);
}
