/*
 * @file		GameScene.h
 * @brief		ゲームシーン
 * @author		戸澤翔太
 * @data		2018/08/18
 */
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

#include <FrameWork/Systems/Renderer/SkyDome.h>
#include <FrameWork/Systems/Renderer/MeshField.h>

#include <FrameWork/Systems/Renderer/Model/MeshRenderer.h>
#include <FrameWork/Systems/Renderer/Sprite/SpriteRenderer.h>

#include <FrameWork/Object/ObjectManager.h>

//-----------------------------------------------------------------------------
//	クラス宣言
//-----------------------------------------------------------------------------
class Light;
class GameScene : public BaseScene, public GUI
{
public:
	GameScene(SceneManager* manager);
	~GameScene(void);

	void Init(void)   override;
	void Uninit(void) override;

	/* @brief	フィールド取得関数
	 * @return	フィールドのポインタ		*/
	MeshField*		GetMeshField(void)		{ return meshField_; }
	/* @brief	ライト取得処理
	 * @return	ライトのポインタ			*/
	Light*			GetLight(void)			{ return light_;     }

private:
	SceneList Update(void) override;

	//! オブジェクトのマネージャー
	ObjectManager*	objectManager_;

	//! 天球
	SkyDome*		sky_;
	//! フィールド
	MeshField*		meshField_;
	//! ライト
	Light*			light_;
};

#endif // _GAME_SCENE_H_
