#include "DragonBite.h"
#include "../Dragon.h"

//! @def	アニメーションの速度を変える(速くする)タイミング
static constexpr int CHANGE_FRAME = 20;

/* @fn		コンストラクタ
 * @brief	変数の初期化		*/
DragonBite::DragonBite(void) : debug_speed_(0), debug_changeFrame_(CHANGE_FRAME)
{
}

/* @fn		デストラクタ
 * @brief	...					*/
DragonBite::~DragonBite(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	(object)	このクラスでは使わない
 * @return	なし				*/
void DragonBite::Init(Object* object)
{
	UNREFERENCED_PARAMETER(object);
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし				*/
void DragonBite::Uninit(void)
{
}

/* @fn		SetMove
 * @brief	実行する準備
 * @param	(mesh)			アニメーションを変更したいメッシュ
 * @param	(animSpeed)		アニメーション速度
 * @param	(animNum)		アニメーションの番号
 * @return	なし
 * @detail	この攻撃特有のアニメーション、速度を設定する		*/
void DragonBite::SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum)
{
	// 既に使用中なら重複防止
	if (enable_) { return; }

	MonsterAttack::SetMove(mesh, animSpeed, animNum);

	// 速度の設定
	animSpeed	 = 0.4f;
	debug_speed_ = animSpeed;

	// アニメーションの設定
	animNum   = static_cast<int>(Dragon::Animation::BITE);

	// 実際の切り替え
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
bool DragonBite::Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd)
{
	// 使わない
	UNREFERENCED_PARAMETER(trans);

	// 移動はさせない
	velocity = VECTOR3(0);

	// 演出用
	frame_++;

	// 一定の時間を超えたらアニメーション速度を変える
#ifdef _SELF_DEBUG
	// デバッグ用
	if (frame_ > debug_changeFrame_)
#else
	if (frame_ > CHANGE_FRAME)
#endif
	{
		animSpeed	 = 0.75f; 
		debug_speed_ = animSpeed;
	}

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
void DragonBite::GuiUpdate(void)
{
	ImGui::Text("frame : %d", frame_);
	ImGui::Text("speed : %.2f", debug_speed_);
	ImGui::DragInt("changeFrame", &debug_changeFrame_);
}
