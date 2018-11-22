/*
 * @file		ItemList.h
 * @brief		�A�C�e���̃��X�g
 * @author		���V�đ�
 * @data		2018/11/22
 */
#ifndef _ITEM_LIST_H_
#define _ITEM_LIST_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

#include "../GameObject/Player/Player.h"

class ItemList : public Object
{
	//! @def	���ɏo��A�C�e��
	enum class BackItem : uint8
	{
		BackRight = 0,
		FrontRight,
		Center,
		FrontLeft,
		BackLeft,
		EMPTY,
		MAX
	};

	//! @def	�{�^��UI
	enum class ButtonUI : uint8
	{
		L = 0,
		MARU,
		SHIKAKU,
		MAX
	};

public:
	ItemList(void);
	~ItemList(void);

	void Init(void)   override;
	void Uninit(void) override;
	void Update(void) override;

	inline void SetPlayer(Player* player) { player_ = player; }

private:
	Player* player_;

	CanvasRenderer back_;
//	CanvasRenderer itemBack_;
	CanvasRenderer itemName_;
	CanvasRenderer backItemBack_[static_cast<int>(BackItem::MAX)];

	CanvasRenderer ui_[static_cast<int>(ButtonUI::MAX)];

	int flag_;
	int cnt_;
};

#endif // _ITEM_LIST_H_
