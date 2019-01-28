/*
 * @file		PlayerItemList.h
 * @brief		���S����
 * @author		���V�đ�
 * @data		2018/11/14
 */
#ifndef _PLAYER_ITEM_LIST_H_
#define _PLAYER_ITEM_LIST_H_

#include <FrameWork/Define/Define.h>

//! @enum	�A�C�e���̃��X�g
enum class ItemID : int8
{
	UNKNOWN  = -1,
	Recovery = 0,
	Rations,
	MAX
};

struct ITEM_LIST
{
	ItemID itemID;
	uint8  possession;
};

class PlayerItemList
{
public:
	static constexpr uint8 MAX_ITEM = 5;

	/* @brief	�R���X�g���N�^		*/
	PlayerItemList(void);
	/* @brief	�f�X�g���N�^		*/
	~PlayerItemList(void);


	
	/* @brief	����������
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Init(void);
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void);
		
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�
	 * @detail	�v���C���[����Ă΂��	*/
	void GuiUpdate(void);

	/* @brief	�A�C�e���̎擾����
	 * @param	(addItem)	�ǉ�����A�C�e���̏��
	 * @return	�����i����t�̎�false	*/
	bool AddItem(ITEM_LIST addItem);

	/* @brief	�����̃A�C�e���̎擾����
	 * @param	(arrayNum)		�z��ԍ�
	 * @return	�A�C�e���̏��			*/
	inline ITEM_LIST GetItemInfo(int arrayNum) { return itemList_[arrayNum]; }

private:
	// �A�C�e���̃��X�g
	ITEM_LIST	itemList_[MAX_ITEM];
};

#endif // _DIE_STATE_H_
