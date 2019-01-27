#include "PlayerItemList.h"

PlayerItemList::PlayerItemList(void) : 
	player_(nullptr)
	, current_(0)
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

void PlayerItemList::Init(Player* player)
{
	if (!player) { return; }
	player_ = player;
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

void PlayerItemList::SelectItem(bool right)
{
	current_ += (right) ? 1 : -1;

	// Out of Range防止
	if (current_ >= MAX_ITEM)	{ current_ = 0;				}
	else if (current_ <= 0)		{ current_ = MAX_ITEM - 1;	}
}

void PlayerItemList::GuiUpdate(void)
{
}
