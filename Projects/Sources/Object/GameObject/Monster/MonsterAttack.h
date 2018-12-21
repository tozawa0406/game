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

class MonsterAttack : public GUI
{
public:
	/* @brief	コンストラクタ		*/
	MonsterAttack(void)	: GUI(Systems::Instance(), nullptr, "attack"), enable_(false), frame_(0), debug_nextFrame_(false)	{}
	/* @brief	デストラクタ		*/
	virtual ~MonsterAttack(void)	{}

	/* @brief	初期化処理			*/
	virtual void Init(GameObject* monster) { monster_ = monster; }
	/* @brief	後処理				*/
	virtual void Uninit(void) = 0;
	/* @brief	準備処理			*/
	virtual void SetMove(void) { enable_ = true; frame_ = 0; }
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
	//! 使用フラグ
	bool	enable_;
	//! 攻撃中の当たり判定等判断用
	int		frame_;
	//! デバッグのフレーム送り
	bool	debug_nextFrame_;
	//! 攻撃を行う元のモンスターのポインタ
	GameObject* monster_;
};

#endif // _MONSTAER_ATTACK_H_
