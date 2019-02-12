/*
 * @file		CampScene.h
 * @brief		ゲームシーン
 * @author		戸澤翔太
 * @data		2018/08/18
 */
#ifndef _CAMP_SCENE_H_
#define _CAMP_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>

#include <FrameWork/Systems/Renderer/SkyDome.h>
#include <FrameWork/Systems/Renderer/MeshField.h>

#include <FrameWork/Systems/Renderer/Model/MeshRenderer.h>
#include <FrameWork/Systems/Renderer/Sprite/SpriteRenderer.h>

#include <FrameWork/Object/ObjectManager.h>
#include "../Object/StaticObject/Goal.h"

#include "../Object/GameObject/AttackManager.h"
#include "../Object/UI/Tutorial/TutorialManager.h"

class CampScene : public BaseScene, public GUI
{
public:
	/* @brief	コンストラクタ		*/
	CampScene(void);
	/* @brief	デストラクタ		*/
	~CampScene(void);


	
	/* @brief	初期化処理
	 * @param	(sceneNum)	シーン番号
	 * @return	なし				*/
	void Init(SceneList sceneNum) override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし					*/
	void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	シーン遷移番号			*/
	SceneList Update(void) override;

private:
	/* @brief	フィールドの生成
	 * @sa		Init()
	 * @param	なし
	 * @return	なし					*/
	void CreateField(void);

	//! オブジェクトのマネージャー
	ObjectManager*	objectManager_;
	//! 遷移位置
	Goal*			goal_;

	//! 天球
	SkyDome*		sky_;
	//! 攻撃管理クラス
	AttackManager*	attackManager_;
	//! チュートリアル
	TutorialManager*	tutorialManager_;
};

#endif // _CAMP_SCENE_H_
