/*
 * @file		ResourceInfoManager.h
 * @brief		シーン情報管理クラス
 * @author		戸澤翔太
 * @data		2018/08/18
 */
#ifndef _RESOURCE_INFO_MANAGER_H_
#define _RESOURCE_INFO_MANAGER_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Windows/Windows.h>
#include "00_EachScene.h"
#include "ResourcesTexture.h"
#include "ResourcesModel.h"
#include "ResourcesSound.h"

class ResourceInfoManager
{
public:
	/* @brief	コンストラクタ		*/
	ResourceInfoManager(void);
	/* @brief	デストラクタ		*/
	~ResourceInfoManager(void);

	/* @brief	初期化処理
	 * @param	なし
	 * @return	成功失敗			*/
	HRESULT Init(Windows* window);

	/* @brief	テクスチャのロード情報の取得
	 * @param	(state)		シーン
	 * @param	(size)		そのシーン分のサイズ
	 * @param	(max)		ベースも含めたサイズ
	 * @return	テクスチャ名		*/
	const string* LoadTexture(SceneList state, int& size, int& max) const;
	
	/* @brief	モデルのロード情報の取得
	 * @param	(state)		シーン
	 * @param	(size)		そのシーン分のサイズ
	 * @param	(max)		ベースも含めたサイズ
	 * @return	モデル名			*/
	const string* LoadModel(SceneList state, int& size, int& max) const;
	
	/* @brief	モデルアニメーションのロード情報の取得
	 * @param	(state)		シーン
	 * @param	(size)		そのシーン分のサイズ
	 * @param	(max)		ベースも含めたサイズ
	 * @return	モデルアニメーション名		*/
	const ANIMATION_INFO* LoadModelAnimation(SceneList state, int& size, int& max) const;

	/* @brief	サウンドのロード情報の取得
	 * @param	(state)		シーン
	 * @param	(size)		そのシーン分のサイズ
	 * @param	(max)		ベースも含めたサイズ
	 * @return	サウンド名とBGM/SE区分		*/
	const SOUNDPARAM* LoadSound(SceneList state, int& size, int& max) const;

	/* @brief	白テクスチャの名前取得
	 * @param	なし
	 * @return	白テクスチャの名前	*/
	const string& GetWhiteTextureName(void) const;

private:
	Resources::Texture	texture_;
	Resources::Model	model_;
	Resources::Sound	sound_;
};

#endif // _RESOURCE_INFO_MANAGER_
