/*
 * @file		ResourceInfoManager.h
 * @brief		シーン情報管理クラス
 * @author		戸澤翔太
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
	/* @brief	コンストラクタ		*/
	ResourceInfoManager(void);
	/* @brief	デストラクタ		*/
	~ResourceInfoManager(void);



	const SOUNDPARAM* LoadSound(SceneList state, int& size, int& max) const;

private:
	Resources::Sound sound_;
};

#endif // _RESOURCE_INFO_MANAGER_
