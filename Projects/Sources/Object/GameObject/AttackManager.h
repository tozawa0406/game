/*
 * @file		AttackManager.h
 * @brief		攻撃管理クラス
 * @author		戸澤翔太
 * @data		2019/02/03
 */
#ifndef _ATTACK_MANAGER_H_
#define _ATTACK_MANAGER_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include <unordered_map>
#include <random>

class AttackManager : public GUI
{
public:

	/* @brief	コンストラクタ		*/
	AttackManager(void) : GUI(Systems::Instance(), nullptr, "attackManager") {}
	/* @brief	デストラクタ		*/
	~AttackManager(void) {}

	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void) {}
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) { list_.clear(); }

	uint8 CreateAttackID(void) const
	{
		for (;;)
		{
			std::random_device randDev;
			uint8 attack = static_cast<uint8>(randDev() % 255);

			if (!CheckList(attack)) { continue; }
			return attack;
		}
	}

	/* @brief	リスト内に存在するか
	 * param	(id)		チェックするID
	 * @return	なければtrue		*/
	inline bool CheckList(uint8 id) const { return (list_.count(id) == 0) ? true : false; }

	/* @brief	追加するID
	 * @param	(id)	追加するID
	 * @return	なし				*/
	inline void AddKey(uint8 id) { list_[id] = true; }

	/* @brief	IDの削除
	 * @param	(id)	削除するID
	 * @return	なし				*/
	inline void DeleteKey(uint8 id) { list_.erase(id); }

	void GuiUpdate(void) override
	{
		for (auto& l : list_)
		{
			ImGui::Text("%d", l.first);
		}
	}

private:
	//! 攻撃IDのリスト
	std::unordered_map<uint8, bool> list_;
};

#endif // _ATTACK_MANAGER_H_
