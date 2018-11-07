#include "DragonTakeOff.h"
#include "../Dragon.h"

/* @fn		コンストラクタ
 * @brief	変数の初期化		*/
DragonTakeOff::DragonTakeOff(void) : fly_(false), debug_velocity_(0)
{
}

/* @fn		デストラクタ
 * @brief	...					*/
DragonTakeOff::~DragonTakeOff(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	(object)	このクラスでは使わない
 * @return	なし				*/
void DragonTakeOff::Init(Object* object)
{
	UNREFERENCED_PARAMETER(object);
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし				*/
void DragonTakeOff::Uninit(void)
{
}

/* @fn		SetMove
 * @brief	実行する準備
 * @param	(mesh)			アニメーションを変更したいメッシュ
 * @param	(animSpeed)		アニメーション速度
 * @param	(animNum)		アニメーションの番号
 * @return	なし
 * @detail	この攻撃特有のアニメーション、速度を設定する		*/
void DragonTakeOff::SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum)
{
	// 既に使用中なら重複防止
	if (enable_) { return; }

	MonsterAttack::SetMove(mesh, animSpeed, animNum);

	// 速度の設定
	animSpeed = fly_ ? -0.75f : 0.75f;

	// アニメーションの設定
	animNum = static_cast<int>(Dragon::Animation::TAKE_OFF);

	// 実際のアニメーションの切り替え
	mesh.ChangeAnimation(animNum, 15);
}

/* @fn		Update
 * @brief	更新処理
 * @param	(tarns)		姿勢の変更がある場合
 * @param	(velocity)	速度の変更がある場合()
 * @param	(mesh)		アニメーションを変更したいメッシュ
 * @param	(animSpeed)	アニメーション速度
 * @param	(animNum)	アニメーションの番号
 * @param	(animEnd)	アニメーションが終わったかどうか
 * @return	攻撃が終了したらtrue
 * @detail	姿勢変更はなし、移動しないよう速度は0に固定
			一定時間経過後、アニメーションの速度を上げる、アニメーションが終了したら元に戻って終了		*/
bool DragonTakeOff::Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd)
{
	// 使わない
	UNREFERENCED_PARAMETER(trans);

	int sign = (fly_) ? -1 : 1;
	velocity.x = 0;
	velocity.z = 0;
	velocity.y += 0.1f * sign;

	debug_velocity_ = velocity.y;

	// 演出用
	frame_++;

	// アニメーション終了
	if (animEnd)
	{
		// 元に戻す
		animSpeed = 0.75f;
		animNum = static_cast<int>(Dragon::Animation::WAIT);
		enable_ = false;
		mesh.ChangeAnimation(animNum, 5, true);
		return true;
	}
	return false;
}

/* @fn		GuiUpdate
 * @brief	Guiの更新処理
 * @param	なし
 * @return	なし
 * @detail	攻撃元オブジェクトから呼ばれる		*/
void DragonTakeOff::GuiUpdate(void)
{
	ImGui::Text("frame : %d", frame_);
	ImGui::Text("velocity : %.2f", debug_velocity_);
}
