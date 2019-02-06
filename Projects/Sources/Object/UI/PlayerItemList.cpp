#include "PlayerItemList.h"
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include <FrameWork/Scene/SceneManager.h>

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

void PlayerItemList::Init(BaseScene* scene)
{
	if (!scene) { return; }
	scene_ = scene;

	if (const auto& sceneManager = scene->GetManager())
	{
		if (const auto& load = sceneManager->GetDontDestroyOnLoad())
		{
			for (int i = 0; i < MAX_ITEM; ++i)
			{
				char buf[3];
				sprintf_s(buf, "%d", i);
				string key = "ItemID_";
				key += buf;
				int a = load->Load<int>(key, -1);
				if (a >= 0) { itemList_[i].itemID = static_cast<ItemID>(a); }
				key = "ItemPossession_";
				key += buf;
				a = load->Load<int>(key, 0);
				if (a != 0) { itemList_[i].possession = static_cast<int8>(a); }
			}
		}
	}
}

void PlayerItemList::Uninit(void)
{
	if (!scene_) { return; }

	if (const auto& sceneManager = scene_->GetManager())
	{
		if (const auto& load = sceneManager->GetDontDestroyOnLoad())
		{
			for (int i = 0; i < MAX_ITEM; ++i)
			{
				char buf[3];
				sprintf_s(buf, "%d", i);
				string key = "ItemID_";
				key += buf;
				load->Save(key, static_cast<int>(itemList_[i].itemID));
				key = "ItemPossession_";
				key += buf;
				load->Save(key, static_cast<int>(itemList_[i].possession));
			}
		}
	}

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

void PlayerItemList::UseItem(int arrayNum)
{
	if (arrayNum < 0 || MAX_ITEM <= arrayNum) { return; }

	auto& list = itemList_[arrayNum];
	list.possession--;
	if (list.possession == 0)
	{
		list.itemID = ItemID::UNKNOWN;
		list.possession = 0;
	}
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
