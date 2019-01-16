/*
 * @file		ResourceInfoManager.h
 * @brief		�V�[�����Ǘ��N���X
 * @author		���V�đ�
 * @data		2018/08/18
 */
#ifndef _RESOURCE_INFO_MANAGER_H_
#define _RESOURCE_INFO_MANAGER_H_

#include <FrameWork/Define/Define.h>
#include "00_EachScene.h"
#include "Resources.h"

class ResourceInfoManager
{
public:
	/* @brief	�R���X�g���N�^		*/
	ResourceInfoManager(void);
	/* @brief	�f�X�g���N�^		*/
	~ResourceInfoManager(void);



	const SOUNDPARAM* LoadSound(SceneList state, int& size, int& max) const;

private:
	Resources::Sound sound_;
};

#endif // _RESOURCE_INFO_MANAGER_
