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

/* @brief	コンストラクタ			*/
DragonWingAttack::DragonWingAttack(void) : debug_speed_(0)
	, debug_changeFrame_(CHANGE_FRAME)
{
	for (auto& c : collider_) { c = nullptr; }
}

/* @brief	デストラクタ			*/
DragonWingAttack::~DragonWingAttack(void)
{
}

/* @brief	初期化処理
 * @param	(object)	当たり判定の親クラスに登録
 * @return	なし					*/
void DragonWingAttack::Init(GameObject* monster)
{
	MonsterAttack::Init(monster);

	// 当たり判定の生成
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& renderer = systems->GetRenderer())
		{
			if (DirectX11Wrapper* wrapper = static_cast<DirectX11Wrapper*>(renderer->GetWrapper()))
			{
				const auto& model = wrapper->GetModel(static_cast<int>(Model::Game::DRAGON));

				int num = static_cast<int>(Wing::CLAW_L);
				collider_[num] = new Collider3D::OBB(monster);
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
					const auto& s = monster->GetTransform().scale;
					collider_[num]->SetOffsetPosition(COLLISION_OFFSET_POS_CLAW_L * s);
					collider_[num]->SetOffsetRotation(COLLISION_OFFSET_ROT_CLAW_L);
					collider_[num]->SetSize(COLLISION_SIZE_CLAW_L * s);
					collider_[num]->SetRendererColor(COLOR(1, 0, 0, 1));
					collider_[num]->SetEnable(false);
				}

				num = static_cast<int>(Wing::CLAW_R);
				collider_[num] = new Collider3D::OBB(monster);
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
					const auto& s = monster->GetTransform().scale;
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

/* @brief	後処理
 * @param	なし
 * @return	なし					*/
void DragonWingAttack::Uninit(void)
{
	for (auto& c : collider_) { DeletePtr(c); }
}

/* @brief	実行する準備
 * @param	なし
 * @return	なし
 * @detail	この攻撃特有のアニメーション、速度を設定する		*/
void DragonWingAttack::SetMove(void)
{
	// 既に使用中なら重複防止
	if (enable_ || !monster_) { return; }

	auto& meshAnim = monster_->GetMeshAnimation();

	MonsterAttack::SetMove();

	// 速度の設定
	meshAnim.animSpeed	 = 0.6f;
	debug_speed_ = meshAnim.animSpeed;

	// アニメーションの設定
	meshAnim.animation = static_cast<int>(Dragon::Animation::WING_ATTACK);

	// 実際のアニメーションの切り替え
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, 15);
}

/* @brief	更新処理
 * @param	なし
 * @return	攻撃が終了したらtrue
 * @detail	姿勢変更はなし、移動しないよう速度は0に固定
			一定時間経過後、アニメーションの速度を上げる、アニメーションが終了したら元に戻って終了		*/
bool DragonWingAttack::Update(void)
{
	if (!monster_) { return true; }

	// 移動させない
	monster_->SetVelocity(VECTOR3(0));

	// 演出用
	frame_++;

	auto& meshAnim = monster_->GetMeshAnimation();

	// 一定の時間を超えたらアニメーション速度を変える
#ifdef _SELF_DEBUG
	// デバッグ用
	if (frame_ > debug_changeFrame_)
#else
	if (frame_ > CHANGE_FRAME)
#endif
	{
		meshAnim.animSpeed    = 0.75f;
		debug_speed_ = meshAnim.animSpeed;

		for (auto& c : collider_) { c->SetEnable(true); }
	}

	if (frame_ > END_ATTACK)
	{
		for (auto& c : collider_) { c->SetEnable(false); }
	}

	for (auto& collider : collider_)
	{
		const auto& hits = collider->Hit();
		for (auto& hit : hits)
		{
			if (hit->GetTag() == Object::Tag::PLAYER)
			{
				static_cast<GameObject*>(hit)->Hit(40);
			}
		}
	}

	// アニメーション終了
	if (monster_->IsEndAnim())
	{
		// 元に戻す
		meshAnim.animSpeed = 0.75f;
		meshAnim.animation = static_cast<int>(Dragon::Animation::WAIT1);
		enable_ = false;
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, 5, true);
		return true;
	}
	return false;
}

/* @brief	モーションの終了処理
 * @param	なし
 * @return	なし			*/
void DragonWingAttack::EndMove(void)
{
	for (auto& collider : collider_)
	{
		collider->SetEnable(false);
	}
}

/* @brief	Guiの更新処理
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
