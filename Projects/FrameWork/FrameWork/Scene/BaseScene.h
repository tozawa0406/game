//-----------------------------------------------------------------------------
//
//	シーン基底[BaseScene.h]
//	Auther : 戸澤翔太
//																	2018/02/27
//-----------------------------------------------------------------------------
#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

#include "../Define/Define.h"

#include "../Systems/Renderer/Sprite/CanvasRenderer.h"
#include "../Systems/Input/Controller.h"

#include "../Systems/DebugSetting/Debug.h"

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class Scene
{
public:
	enum class Num			// シーン数
	{
		TITLE = 0,		// タイトル
		GAME,			// ゲーム
		RESULT,			// リザルト
		MAX
	};
};

class SceneManager;
class BaseScene
{
public:
	BaseScene(SceneManager* manager);
	virtual ~BaseScene(void) {};					// デストラクタ

	virtual int				Update(void)	 { return 0; }			// 更新処理
	virtual SceneManager*	GetManager(void) { return manager_; }	// 親の取得

	Controller* GetCtrl(int i);						// コントローラの取得
	Systems*    GetSystems(void) { return systems_; }

protected:
	SceneManager* manager_;		// 親
	Systems*      systems_;
};

#endif // _BASE_SCENE_H_
