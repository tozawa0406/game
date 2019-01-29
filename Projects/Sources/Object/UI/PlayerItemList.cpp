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
		// �A�C�e�����X�g�̋󂫂��m�F
		if (itemList_[i].itemID == ItemID::UNKNOWN && inside == 0) { inside = i; }

		// ����A�C�e��������Ώ����������Z
		if (itemList_[i].itemID == addItem.itemID)
		{
			itemList_[i].possession += addItem.possession;

			return LimitPossesion(itemList_[i]);
		}
	}

	// 0�Ԗڂ͖��Œ�Ȃ̂ŏ�������t
	if (inside == 0) { return addItem.possession; }

	// �󂫂ɂԂ�����ł�邺
	itemList_[inside] = addItem;
	return LimitPossesion(itemList_[inside]);
}

int PlayerItemList::LimitPossesion(ITEM_LIST& list)
{
	int possession = list.possession;
	ItemInfo itemInfo;
	// ����������
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
