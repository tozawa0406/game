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

class MonsterAttack : public GUI
{
public:
	MonsterAttack(void)	: GUI(Systems::Instance(), nullptr, "attack"), enable_(false), frame_(0), debug_nextFrame_(false)	{}
	virtual ~MonsterAttack(void)	{}

	virtual void Init(Object* object) = 0;
	virtual void Uninit(void) = 0;
	virtual void SetMove(MeshRenderer& mesh, float& animSpeed, int& animNum) { enable_ = true; frame_ = 0; UNREFERENCED_PARAMETER(mesh); UNREFERENCED_PARAMETER(animNum); UNREFERENCED_PARAMETER(animSpeed); }
	virtual bool Update(Transform& trans, VECTOR3& velocity, MeshRenderer& mesh, float& animSpeed, int& animNum, bool animEnd) = 0;

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
};

#endif // _MONSTAER_ATTACK_H_
