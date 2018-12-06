/*
 * @file		EachScene.h
 * @brief		シーン情報管理クラス
 * @author		戸澤翔太
 * @data		2018/08/18
 */
#ifndef _EACH_SCENE_H_
#define _EACH_SCENE_H_

#include <FrameWork/Define/Define.h>

//! @enum	シーン遷移フラグ
enum class SceneList : uint8
{
	NEXT		= 255,	// 次のシーン
	NOTCHANGE	= 0,	// 遷移なし
	TITLE,				// タイトル
	GAME,				// ゲーム
	RESULT,				// リザルト
	MAX
};

class SceneManager;
class BaseScene;
class EachScene
{
public:
	EachScene(void);
	~EachScene(void);

	void		ChangeScene(SceneList sceneChange, SceneList& sceneNum);
	int 		ChangePause(int returnPause);
	BaseScene*  CreateScene(SceneManager* manager, SceneList sceneNum);
	BaseScene*	CreatePause(SceneManager* manager, SceneList sceneNum);
};

#endif // _EACH_SCENE
