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

void PlayerItemList::SelectItem(bool right)
{
	current_ += (right) ? 1 : -1;

	// Out of Range�h�~
	if (current_ >= MAX_ITEM)	{ current_ = 0;				}
	else if (current_ <= 0)		{ current_ = MAX_ITEM - 1;	}
}

void PlayerItemList::GuiUpdate(void)
{
}