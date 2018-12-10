//-----------------------------------------------------------------------------
//
//	シーン処理[SceneManager.h]
//	Auther : 戸澤翔太
//																	2018/02/27
//-----------------------------------------------------------------------------
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "../Define/Define.h"

#include <Scene/EachScene.h>
#include "BaseScene.h"
#include "../Systems/Renderer/CanvasRenderer/CanvasRendererImage.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/DebugSetting/Debug.h"

#include "../Systems/Loading.h"

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class CameraManager;
class SceneManager : public Interface
{
	static constexpr int CAMERA_DEBUG = 1;

	static constexpr int SCENE_FADE_IN  = 30;				//フェードの時間(フレーム単位)
	static constexpr int SCENE_FADE_OUT = 30;

public:

	SceneManager(Systems* systems);				// コンストラクタ
	~SceneManager(void);						// デストラクタ
	HRESULT Init(void)   override;
	void    Uninit(void) override;
	void    Update(void) override;				// 更新処理
	void    Draw(void)   override;				// 描画処理
	void    Change(SceneList scene = SceneList::NEXT);	// 遷移準備処理

	// Getter
	SceneList		GetSceneNum(void)      { return displayMode_;	}		// 現在シーンの番号
	BaseScene*		GetScene(void)         { return scene_;			}		// 現在のシーンのポインタ
	BaseScene*		GetPause(void)         { return pause_;			}		// ポーズシーンへのポインタ
	CameraManager*	GetCameraManager(void) { return camera_;		}		// カメラマネージャーへのポインタ
	bool			isPause(void)          { return isPause_;		}		// ポーズ状態フラグ
	void			SetPause(bool pause)   { isPause_ = pause;		}		// ポーズ状態フラグ

private:
	BaseScene* scene_;		// シーン
	BaseScene* pause_;		// ポーズ
	EachScene* eachScene_;	// シーンの情報を管理

	void SceneUpdate(void);			// シーンの更新処理
	void Fade(void);				// フェード処理
	void ChangeActual(void);		// シーン遷移処理
	void ForceSceneChange(void);	// 強制シーン遷移(デバッグ用)

	SceneList	sceneChange_;		// シーン遷移
	SceneList	displayMode_;		// 画面の状態
	int			fadeCnt_;			// フェード進行
	bool		isPause_;			// ポーズの状態

	CanvasRenderer::Image fade_;

	Loading*		loading_;
	bool			startLoad_;
	CameraManager* camera_;		// カメラマネージャー
};

#endif // _SCENE_MANAGER_H_
