/*
 * @file		ItemInfo.h
 * @brief		�A�C�e���̏��ꗗ
 * @author		���V�đ�
 * @data		2018/11/22
 */
#ifndef _ITEM_INFO_H_
#define _ITEM_INFO_H_

#include <FrameWork/Define/Define.h>
#include "../../Scene/ResourcesTexture.h"

 //! @enum	�A�C�e���̃��X�g
enum class ItemID : int8
{
	UNKNOWN = -1,
	Recovery = 0,
	Rations,
	MAX
};

struct ITEM_INFO
{
	ItemID	id;
	int		possessionMax;
	int		texNum;
};

class ItemInfo
{
public:
	const ITEM_INFO info[static_cast<int>(ItemID::MAX)] =
	{
		{ ItemID::Recovery, 10, static_cast<int>(Resources::Texture::Camp::ITEM_RECOVERY)	},
		{ ItemID::Rations , 10, static_cast<int>(Resources::Texture::Camp::ITEM_RATION)		}
	};
};

#endif // _ITEM_INFO_H_
