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
	/* @brief	コンストラクタ		*/
	EachScene(void);
	/* @brief	デストラクタ		*/
	~EachScene(void);



	/* @brief	シーン管理の変数の値を変更する
	 * @param	(sceneChange)	シーン遷移フラグ
	 * @param	(sceneNum)		シーン管理番号
	 * @return	なし					*/
	void		ChangeScene(SceneList sceneChange, SceneList& sceneNum);

		/* @brief	ポーズ終了時のコマンド
	 * @param	(returnPause)		ポーズの返り値
	 * @return	他のシーンに移る場合は0以外				*/
	int 		ChangePause(int returnPause);

	/* @brief	シーンのnew処理
	 * @param	(sceneNum)		シーン管理番号
	 * @return	シーン番号に応じたシーンのnew処理		*/
	BaseScene*  CreateScene(SceneManager* manager, SceneList sceneNum);
	
	/* @fn		CreatePause
	 * @brief	ポーズシーンのnew処理
	 * @param	(sceneNum)		シーン管理番号
	 * @return	ポーズのnew処理、使わない場合はnullptr		*/
	BaseScene*	CreatePause(SceneManager* manager, SceneList sceneNum);
};

#endif // _EACH_SCENE
