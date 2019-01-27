/*
 * @file		PlayerItemList.h
 * @brief		死亡処理
 * @author		戸澤翔太
 * @data		2018/11/14
 */
#ifndef _PLAYER_ITEM_LIST_H_
#define _PLAYER_ITEM_LIST_H_

#include "Player.h"

//! @enum	アイテムのリスト
enum class ItemID : int8
{
	UNKNOWN  = -1,
	Recovery = 0,
};

struct ITEM_LIST
{
	ItemID itemID;
	uint8  possession;
};

class PlayerItemList
{
	static constexpr int8 MAX_ITEM = 5;
public:
	/* @brief	コンストラクタ		*/
	PlayerItemList(void);
	/* @brief	デストラクタ		*/
	~PlayerItemList(void);


	
	/* @brief	初期化処理
	 * @param	(player)	プレイヤーへのポインタ
	 * @return	なし				*/
	void Init(Player* player);
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void);
		
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし
	 * @detail	プレイヤーから呼ばれる	*/
	void GuiUpdate(void);

	/* @brief	アイテムの取得処理
	 * @param	(addItem)	追加するアイテムの情報
	 * @return	所持品が一杯の時false	*/
	bool AddItem(ITEM_LIST addItem);

	/* @brief	現在表示アイテムの変更
	 * @param	(right)		右回転？
	 * @return	なし					*/
	void SelectItem(bool right);

	/* @brief	中央のアイテムの取得処理
	 * @param	なし
	 * @return	アイテムの情報			*/
	inline ITEM_LIST GetItemInfo(void) { return itemList_[current_]; }

private:
	// プレイヤー
	Player*		player_;
	// 現在位置
	int8		current_;
	// アイテムのリスト
	ITEM_LIST	itemList_[MAX_ITEM];
};

#endif // _DIE_STATE_H_
