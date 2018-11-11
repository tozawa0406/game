#include "DragonWingAttack.h"
#include "../Dragon.h"
#include <FrameWork/Graphics/DirectX11/DirectX11Wrapper.h>

//! @def	アニメーションの速度を変える(速くする)タイミング
static constexpr int CHANGE_FRAME = 18;
//! @def	攻撃の終了
static constexpr int END_ATTACK = 45;

//! @def	左爪翼のボーンの名前
static const     string  BONE_CLAW_L = "WingClaw2_L";
static const     VECTOR3 COLLISION_OFFSET_POS_CLAW_L = VECTOR3(0, -3, -4);
static const     VECTOR3 COLLISION_OFFSET_ROT_CLAW_L = VECTOR3(0, -2, 0);
static const     VECTOR3 COLLISION_SIZE_CLAW_L = VECTOR3(7.1f, 2.1f, 2.1f);
//! @def	右爪翼のボーンの名前
static const     string  BONE_CLAW_R = "WingClaw2_R";
static const     VECTOR3 COLLISION_OFFSET_POS_CLAW_R = VECTOR3(0, 3, 4);
static const     VECTOR3 COLLISION_OFFSET_ROT_CLAW_R = VECTOR3(0, -2, 0);
static const     VECTOR3 COLLISION_SIZE_CLAW_R = VECTOR3(7.1f, 2.1f, 2.1f);

/* @fn		コンストラクタ
 * @brief	変数の初期化		*/
DragonWingAttack::DragonWingAttack(void) : debug_speed_(0), debug_changeFrame_(CHANGE_FRAME)
{
	for (auto& c : collider_) { c = nullptr; }
}

/* @fn		デストラクタ
 * @brief	...					*/
DragonWingAttack::~DragonWingAttack(void)
{
}

/* @fn		Init
 * @brief	初期化処理
 * @param	(object)	当たり判定の親クラスに登録
 * @return	なし				*/
void DragonWingAttack::Init(Object* object)
{
	// 当たり判定の生成
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& renderer = systems->GetRenderer())
		{
			if (DirectX11Wrapper* wrapper = static_cast<DirectX11Wrapper*>(renderer->GetWrapper()))
			{
				const auto& model = wrapper->GetModel(static_cast<int>(Model::Game::DRAGON));

				int num = static_cast<int>(Wing::CLAW_L);
				collider_[num] = new Collider3D::OBB(object);
				if (collider_[num])
				{
					for (auto& bone : model.bone)
					{
						if (bone.name == BONE_CLAW_L)
						{
							collider_[num]->SetParentMtx(&model.transMtx, &bone.nowBone);
							break;
						}
					}
					const auto& s = object->GetTransform().scale;
					collider_[num]->SetOffsetPosition(COLLISION_OFFSET_POS_CLAW_L * s);
					collider_[num]->SetOffsetRotation(COLLISION_OFFSET_ROT_CLAW_L);
					collider_[num]->SetSize(COLLISION_SIZE_CLAW_L * s);
					collider_[num]->SetRendererColor(COLOR(1, 0, 0, 1));
					collider_[num]->SetEnable(false);
				}

				num = static_cast<int>(Wing::CLAW_R);
				collider_[num] = new Collider3D::OBB(object);
				if (collider_[num])
				{
					for (auto& bone : model.bone)
					{
						if (bone.name == BONE_CLAW_R)
						{
							collider_[num]->SetParentMtx(&model.transMtx, &bone.nowBone);
							break;
						}
					}
					const auto& s = object->GetTransform().scale;
					collider_[num]->SetOffsetPosition(COLLISION_OFFSET_POS_CLAW_R * s);
					collider_[num]->SetOffsetRotation(COLLISION_OFFSET_ROT_CLAW_R);
					collider_[num]->SetSize(COLLISION_SIZE_CLAW_R * s);
					collider_[num]->SetRendererColor(COLOR(1, 0, 0, 1));
					collider_[num]->SetEnable(false);
				}
			}
		}
	}

}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし				*/
void DragonWingAttack::Uninit(void)
{
	for (auto& c : collider_) { DeletePtr(c); }
}

/* @fn		SetMove
 * @brief	実行する準備
 * @param	(mesh)			アニメーションを変更したいメッシュ
 * @param	(animSpeed)		アニメーション速度
 * @param	(animNum)		アニメーションの番号
 * @return	なし
 * @detail	この攻撃特有のアニメーション、速度を設定する		*/
void DragonWingAttack::SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum)
{
	// 既に使用中なら重複防止
	if (enable_) { return; }

	MonsterAttack::SetMove(mesh, animSpeed, animNum);

	// 速度の設定
	animSpeed	 = 0.6f;
	debug_speed_ = animSpeed;

	// アニメーションの設定
	animNum = static_cast<int>(Dragon::Animation::WING_ATTACK);

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
bool DragonWingAttack::Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd)
{
	// 使わない
	UNREFERENCED_PARAMETER(trans);

	// 移動させない
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
		animSpeed    = 0.75f;
		debug_speed_ = animSpeed;

		for (auto& c : collider_) { c->SetEnable(true); }
	}

	if (frame_ > END_ATTACK)
	{
		for (auto& c : collider_) { c->SetEnable(false); }
	}

	// アニメーション終了
	if (animEnd)
	{
		// 元に戻す
		animSpeed = 0.75f;
		animNum = static_cast<int>(Dragon::Animation::WAIT1);
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
void DragonWingAttack::GuiUpdate(void)
{
	MonsterAttack::GuiUpdate();

	ImGui::Text("frame : %d", frame_);
	ImGui::Text("speed : %.2f", debug_speed_);
	ImGui::DragInt("changeFrame", &debug_changeFrame_);
}
