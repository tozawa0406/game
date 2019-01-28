#include "PlayerItemList.h"
#include <FrameWork/Systems/DebugSetting/GUI.h>

PlayerItemList::PlayerItemList(void)
{
	for (auto& item : itemList_)
	{
		item.itemID = ItemID::UNKNOWN;
		item.possession = 0;
	}
}

PlayerItemList::~PlayerItemList(void)
{
}

void PlayerItemList::Init(void)
{
}

void PlayerItemList::Uninit(void)
{
}

bool PlayerItemList::AddItem(ITEM_LIST addItem)
{
	int inside = 0;
	for (int i = 1; i < 5; ++i)
	{
		// �A�C�e�����X�g�̋󂫂��m�F
		if (itemList_[i].itemID == ItemID::UNKNOWN && inside == 0) { inside = i; }

		// ����A�C�e��������Ώ����������Z
		if (itemList_[i].itemID == addItem.itemID)
		{
			itemList_[i].possession += addItem.possession;
			return true;
		}
	}

	// 0�Ԗڂ͖��Œ�Ȃ̂ŏ�������t
	if (inside == 0) { return false; }

	// �󂫂ɂԂ�����ł�邺
	itemList_[inside] = addItem;
	return true;
}

void PlayerItemList::GuiUpdate(void)
{
	int i = 0;
	for (auto& item : itemList_)
	{
		if (item.itemID == ItemID::UNKNOWN) { i++; continue; }

		ImGui::Text("array :");
		ImGui::SameLine();
		ImGui::Text("%d", i);
		ImGui::SameLine();
		ImGui::Text(" num :");
		ImGui::SameLine();
		ImGui::Text("%d", item.possession);
		ImGui::SameLine();
		if (item.itemID == ItemID::Recovery)
		{
			ImGui::Text("Recovery");
		}
		else if (item.itemID == ItemID::Rations)
		{
			ImGui::Text("Rations");
		}

		i++;
	}
}
