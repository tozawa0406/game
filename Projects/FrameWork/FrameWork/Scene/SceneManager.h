/*
 * @file		SceneManager.h
 * @brief		シーン管理クラス
 * @author		戸澤翔太
 * @data		2018/02/27
 */
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "../Define/Define.h"

#include <Scene/EachScene.h>
#include "BaseScene.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/DebugSetting/Debug.h"

#include "../Systems/Loading.h"

class CameraManager;
class SceneManager : public Interface
{
	//! @def	カメラデバッグ
	static constexpr int CAMERA_DEBUG = 1;

	//! @def	フェードインの時間(フレーム単位)
	static constexpr int SCENE_FADE_IN  = 30;
	//! @def	フェードアウトの時間(フレーム単位)
	static constexpr int SCENE_FADE_OUT = 30;

public:
	/* @brief	コンストラクタ
	 * @param	(systems)	システム系		*/
	SceneManager(Systems* systems);
	/* @brief	デストラクタ				*/
	~SceneManager(void);



	/* @brief	初期化処理
	 * @param	なし
	 * @return	成功失敗					*/
	HRESULT Init(void)   override;

	/* @brief	後処理
	 * @param	なし
	 * @return	なし						*/
	void    Uninit(void) override;

	/* @brief	更新処理
	 * @param	なし
	 * @return	なし						*/
	void    Update(void) override;

	/* @brief	描画処理
	 * @param	なし
	 * @return	なし						*/
	void    Draw(void)   override;

	/* @brief	遷移準備処理
	 * @param	(scene)		移動したいシーン
	 * @return	なし						*/
	void    Change(SceneList scene = SceneList::NEXT);

	/* @brief	シーン情報管理クラスの取得	*/
	inline EachScene*		GetEachSceneInfo(void) { return eachScene_;		}
	/* @brief	現在シーンの番号取得		*/
	inline SceneList		GetSceneNum(void)      { return displayMode_;	}
	/* @brief	現在のシーンのポインタ取得	*/
	inline BaseScene*		GetScene(void)         { return scene_;			}
	/* @brief	ポーズシーンのポインタ取得	*/
	inline BaseScene*		GetPause(void)         { return pause_;			}
	/* @brief	カメラ管理のポインタ取得	*/
	inline CameraManager*	GetCameraManager(void) { return camera_;		}
	/* @brief	ポーズ状態取得				*/
	inline bool				IsPause(void)          { return isPause_;		}
	/* @brief	ポーズ状態設定				*/
	inline void				SetPause(bool pause)   { isPause_ = pause;		}

private:
	BaseScene* scene_;		//! シーン
	BaseScene* pause_;		//! ポーズ
	EachScene* eachScene_;	//! シーンの情報を管理

	/* @brief	シーンの更新
	 * @param	なし
	 * @return	なし						*/
	void SceneUpdate(void);
	
	/* @brief	フェード処理
	 * @param	なし
	 * @return	なし						*/
	void Fade(void);

	/* @brief	シーン遷移処理
	 * @param	なし
	 * @return	なし						*/
	void ChangeActual(void);
	
	/* @brief	強制シーン遷移(デバッグ用)
	 * @param	なし
	 * @return	なし						*/
	void ForceSceneChange(void);

	SceneList	sceneChange_;		//! シーン遷移
	SceneList	displayMode_;		//! 画面の状態
	int			fadeCnt_;			//! フェード進行
	bool		isPause_;			//! ポーズの状態

	CanvasRenderer::Image fade_;	//! フェードの黒

	Loading*		loading_;		//! ロードクラス
	bool			startLoad_;		//! ロードフラグ
	CameraManager*	camera_;		//1 カメラマネージャー
};

#endif // _SCENE_MANAGER_H_
