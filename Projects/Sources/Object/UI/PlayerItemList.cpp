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
		// アイテムリストの空きを確認
		if (itemList_[i].itemID == ItemID::UNKNOWN && inside == 0) { inside = i; }

		// 同一アイテムがあれば所持数を加算
		if (itemList_[i].itemID == addItem.itemID)
		{
			itemList_[i].possession += addItem.possession;
			return true;
		}
	}

	// 0番目は無固定なので所持数一杯
	if (inside == 0) { return false; }

	// 空きにぶちこんでやるぜ
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
