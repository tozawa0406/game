/*
 * @file		BaseScene.h
 * @brief		シーンの基底クラス
 * @author		戸澤翔太
 * @data		2018/02/27
 */
#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

#include "../Define/Define.h"
#include "../../../Sources/Scene/00_EachScene.h"

#include "../Windows/Windows.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererText.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/GameSystems.h"
#include "../Systems/DebugSetting/Debug.h"

class SceneManager;
class Light;
class MeshField;
class BaseScene
{
public:
	/* @brief	コンストラクタ		*/
	BaseScene(void);
	/* @brief	デストラクタ		*/
	virtual ~BaseScene(void);

	/* @brief	初期化処理			*/
	virtual void Init(SceneList sceneNum) { sceneNum_ = sceneNum; };
	/* @brief	後処理				*/
	virtual void Uninit(void) = 0;

	/* @brief	更新処理			*/
	virtual SceneList		Update(void)	 { return SceneList::NOTCHANGE; }

	/* @brief	マネージャーの取得処理
	 * @param	なし
	 * @return	マネージャーのポインタ		*/
	inline  SceneManager*	GetManager(void) { return manager_; }

	/* @brief	マネージャーの設定処理
	 * @param	(manager)	マネージャーのポインタ
	 * @return	なし				*/
	void SetManager(SceneManager* manager);

	/* @brief	ライト取得処理		*/
	inline Light*		GetLight(void)		{ return light_;		}

	/* @brief	メッシュフィールド取得処理		*/
	inline MeshField*	GetMeshField(void)	{ return meshField_;	}

	/* @brief	シーン番号の取得	*/
	inline SceneList GetSceneNum(void) { return sceneNum_; }

protected:	
	/* @brief	コントローラの取得処理
	 * @param	(i)		コントローラの番号
	 * @return	コントローラのポインタ		*/
	Controller* GetCtrl(int i);

	/* @brief	サウンド取得処理
	 * @param	なし
	 * @return	サウンドのポインタ			*/
	Sound*		GetSound(void);

	//! マネージャー
	SceneManager*	manager_;
	//! システム系
	Systems*		systems_;
	//! ライト
	Light*			light_;
	//! メッシュフィールド
	MeshField*		meshField_;
	//! 自分のシーン番号
	SceneList		sceneNum_;
};

#endif // _BASE_SCENE_H_
