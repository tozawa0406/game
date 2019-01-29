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

int PlayerItemList::AddItem(ITEM_LIST addItem)
{
	int inside = 0;
	for (int i = 1; i < MAX_ITEM; ++i)
	{
		// アイテムリストの空きを確認
		if (itemList_[i].itemID == ItemID::UNKNOWN && inside == 0) { inside = i; }

		// 同一アイテムがあれば所持数を加算
		if (itemList_[i].itemID == addItem.itemID)
		{
			itemList_[i].possession += addItem.possession;

			return LimitPossesion(itemList_[i]);
		}
	}

	// 0番目は無固定なので所持数一杯
	if (inside == 0) { return addItem.possession; }

	// 空きにぶちこんでやるぜ
	itemList_[inside] = addItem;
	return LimitPossesion(itemList_[inside]);
}

int PlayerItemList::LimitPossesion(ITEM_LIST& list)
{
	int possession = list.possession;
	ItemInfo itemInfo;
	// 所持数制限
	int arrayMax = sizeof(itemInfo.info) / sizeof(itemInfo.info[0]);
	for (int j = 0; j < arrayMax; ++j)
	{
		if (itemInfo.info[j].id == list.itemID)
		{
			list.possession = min(list.possession, static_cast<uint8>(itemInfo.info[j].possessionMax));
			break;
		}
	}

	return possession - list.possession;
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
