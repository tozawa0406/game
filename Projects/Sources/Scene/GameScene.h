//-----------------------------------------------------------------------------
//
//	ゲーム[GameScene.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
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
	// マクロ定義
	static constexpr int CREATE_ENEMY_RANGE = 30;
	static constexpr int GET_POINT = 100;

public:
	static constexpr int BILLBOARD_NUM = 100;

	GameScene(SceneManager* manager);		// コンストラクタ
	~GameScene(void);						// デストラクタ

	void Init(void)   override;
	void Uninit(void) override;

	MeshField*		GetMeshField(void)		{ return meshField_; }
	Light*			GetLight(void)			{ return light_;     }

private:
	SceneList Update(void) override;		// 更新処理

	int  frameCnt = 0;		//フレームのカウンタ

	int  ResbornCnt;

	ObjectManager*	objectManager_;

	SkyDome*		sky_;
	MeshField*		meshField_;

	Light*			light_;
};

#endif // _GAME_SCENE_H_
