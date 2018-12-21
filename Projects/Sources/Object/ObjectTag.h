/*
 * @file		ObjectTag.h
 * @brief		�I�u�W�F�N�g�̃^�O
 * @author		���V�đ�
 * @data		2018/12/21
 */
#ifndef _OBJECT_TAG_H_
#define _OBJECT_TAG_H_

#include <FrameWork/Define/Define.h>

 //! @enum	�I�u�W�F�N�g�̃^�O
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
