/*
 * @file		ObjectTag.h
 * @brief		オブジェクトのタグ
 * @author		戸澤翔太
 * @data		2018/12/21
 */
#ifndef _OBJECT_TAG_H_
#define _OBJECT_TAG_H_

#include <FrameWork/Define/Define.h>

 //! @enum	オブジェクトのタグ
enum class ObjectTag : int8
{
	UKOWN = -1,
	PLAYER = 0,
	UI,
	BILLBOARD,
	STATIC,
	ENEMY,
	WAPON,
	MAX
};

#endif // !_OBJECT_TAG_H_
