/*
 * @file		PlayerItemList.h
 * @brief		���S����
 * @author		���V�đ�
 * @data		2018/11/14
 */
#ifndef _PLAYER_ITEM_LIST_H_
#define _PLAYER_ITEM_LIST_H_

#include "Player.h"

//! @enum	�A�C�e���̃��X�g
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
	/* @brief	�R���X�g���N�^		*/
	PlayerItemList(void);
	/* @brief	�f�X�g���N�^		*/
	~PlayerItemList(void);


	
	/* @brief	����������
	 * @param	(player)	�v���C���[�ւ̃|�C���^
	 * @return	�Ȃ�				*/
	void Init(Player* player);
	
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

	/* @brief	���ݕ\���A�C�e���̕ύX
	 * @param	(right)		�E��]�H
	 * @return	�Ȃ�					*/
	void SelectItem(bool right);

	/* @brief	�����̃A�C�e���̎擾����
	 * @param	�Ȃ�
	 * @return	�A�C�e���̏��			*/
	inline ITEM_LIST GetItemInfo(void) { return itemList_[current_]; }

private:
	// �v���C���[
	Player*		player_;
	// ���݈ʒu
	int8		current_;
	// �A�C�e���̃��X�g
	ITEM_LIST	itemList_[MAX_ITEM];
};

#endif // _DIE_STATE_H_
