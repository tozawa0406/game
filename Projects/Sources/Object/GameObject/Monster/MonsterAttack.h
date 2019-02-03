/*
 * @file		MonsterAttack.h
 * @brief		モンスターの攻撃処理クラスの基底クラス
 * @author		戸澤翔太
 * @data		2018/11/06
 */
#ifndef _MONSTER_ATTACK_H_
#define _MONSTER_ATTACK_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>

#include "../GameObject.h"
#include <random>

class MonsterAttack : public GUI
{
public:
	/* @brief	コンストラクタ		*/
	MonsterAttack(void)	: GUI(Systems::Instance(), nullptr, "attack"), enable_(false), debug_nextFrame_(false), attackManager_(nullptr)	{}
	/* @brief	デストラクタ		*/
	virtual ~MonsterAttack(void)	{}

	/* @brief	初期化処理			*/
	virtual void Init(GameObject* monster) { monster_ = monster; }
	/* @brief	後処理				*/
	virtual void Uninit(void) = 0;
	/* @brief	準備処理			*/
	virtual void SetMove(void) 
	{
		enable_ = true;
		if (!attackManager_) { attackManager_ = monster_->GetAttackManager(); }
		attackID_ = attackManager_->CreateAttackID();
	}

	/* @brief	更新処理			*/
	virtual bool Update(void) = 0;
	/* @brief	終了処理			*/
	virtual void EndMove(void)   = 0;

	/* @brief	Gui更新処理			*/
	virtual void GuiUpdate(void) override
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

protected:
	//! デフォルトのアニメーション速度
	static constexpr float DEFAULT_ANIMATION_SPEED = 0.75f;
	//! @def	アニメーション変更速度
	static constexpr int   ANIMATION_CHANGE_FRAME5 = 5;
	//! @def	アニメーション変更速度
	static constexpr int   ANIMATION_CHANGE_FRAME15 = 15;
	//! @def	アニメーション変更速度
	static constexpr int   ANIMATION_CHANGE_FRAME30 = 30;

	const AttackManager*	attackManager_;
	//! 使用フラグ
	bool	enable_;
	//! デバッグのフレーム送り
	bool	debug_nextFrame_;
	//! 攻撃を行う元のモンスターのポインタ
	GameObject* monster_;

	//! 前回の攻撃ID
	uint8 attackID_;
};

#endif // _MONSTAER_ATTACK_H_
