/*
 * @file		BaseScene.h
 * @brief		シーンの基底クラス
 * @author		戸澤翔太
 * @data		2018/02/27
 */
#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

#include "../Define/Define.h"
#include "../../../Sources/Scene/EachScene.h"

#include "../Windows/Windows.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererText.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/GameSystems.h"
#include "../Systems/DebugSetting/Debug.h"

class SceneManager;
class BaseScene
{
public:
	/* @brief	コンストラクタ		*/
	BaseScene(void);
	/* @brief	デストラクタ		*/
	virtual ~BaseScene(void);

	/* @brief	初期化処理			*/
	virtual void Init(void)   = 0;
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
	SceneManager* manager_;
	//! システム系
	Systems*      systems_;
};

#endif // _BASE_SCENE_H_
