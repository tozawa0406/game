/*
 * @file		AttackManager.h
 * @brief		�U���Ǘ��N���X
 * @author		���V�đ�
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

	/* @brief	�R���X�g���N�^		*/
	AttackManager(void) : GUI(Systems::Instance(), nullptr, "attackManager") {}
	/* @brief	�f�X�g���N�^		*/
	~AttackManager(void) {}

	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void) {}
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
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

	/* @brief	���X�g���ɑ��݂��邩
	 * param	(id)		�`�F�b�N����ID
	 * @return	�Ȃ����true		*/
	inline bool CheckList(uint8 id) const { return (list_.count(id) == 0) ? true : false; }

	/* @brief	�ǉ�����ID
	 * @param	(id)	�ǉ�����ID
	 * @return	�Ȃ�				*/
	inline void AddKey(uint8 id) { list_[id] = true; }

	/* @brief	ID�̍폜
	 * @param	(id)	�폜����ID
	 * @return	�Ȃ�				*/
	inline void DeleteKey(uint8 id) { list_.erase(id); }

	void GuiUpdate(void) override
	{
		for (auto& l : list_)
		{
			ImGui::Text("%d", l.first);
		}
	}

private:
	//! �U��ID�̃��X�g
	std::unordered_map<uint8, bool> list_;
};

#endif // _ATTACK_MANAGER_H_
