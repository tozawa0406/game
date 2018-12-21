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

class Light;
class GameScene : public BaseScene, public GUI
{
public:
	/* @brief	コンストラクタ		*/
	GameScene(void);
	/* @brief	デストラクタ		*/
	~GameScene(void);



	/* @brief	初期化処理
	 * @param	なし
	 * @return	なし				*/
	void Init(void)   override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし					*/
	void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	シーン遷移番号			*/
	SceneList Update(void) override;

	/* @brief	フィールド取得関数
	 * @return	フィールドのポインタ		*/
	MeshField*		GetMeshField(void)		{ return meshField_; }
	/* @brief	ライト取得処理
	 * @return	ライトのポインタ			*/
	Light*			GetLight(void)			{ return light_;     }

private:
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
