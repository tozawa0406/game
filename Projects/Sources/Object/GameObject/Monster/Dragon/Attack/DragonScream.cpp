#include "DragonScream.h"
#include "../Dragon.h"

/* @brief	コンストラクタ			*/
DragonScream::DragonScream(void)
{
}

/* @brief	デストラクタ			*/
DragonScream::~DragonScream(void)
{
}

/* @brief	初期化処理
 * @param	(object)	当たり判定の親クラスに登録
 * @return	なし					*/
void DragonScream::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);
}

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void DragonScream::Uninit(void)
{
}

/* @brief	実行する準備
 * @param	なし
 * @return	なし
 * @detail	この攻撃特有のアニメーション、速度を設定する		*/
void DragonScream::SetMove(void)
{
	if (enable_ || !monster_) { return; }

	MonsterAttack::SetMove();

	auto& meshAnim = monster_->GetMeshAnimation();

	meshAnim.animSpeed = 0.75f;
	meshAnim.animation   = static_cast<int>(Dragon::Animation::SCREAM);

	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
}

/* @brief	更新処理
 * @param	なし
 * @return	攻撃が終了したらtrue
 * @detail	姿勢変更はなし、移動しないよう速度は0に固定
			一定時間経過後、アニメーションの速度を上げる、アニメーションが終了したら元に戻って終了		*/
bool DragonScream::Update(void)
{
	if (!monster_) { return true; }

	monster_->SetVelocity(VECTOR3(0));

	if (monster_->IsEndAnim())
	{
		auto& meshAnim = monster_->GetMeshAnimation();

		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 5, true);
		return true;
	}
	return false;
}

/* @fn		EndMove
 * @brief	モーションの終了処理
 * @param	なし
 * @return	なし			*/
void DragonScream::EndMove(void)
{
}
